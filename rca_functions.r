# R code to build a RIvpacs-type model. ;
# Includes computing dissimilarity matrix, clustering, cluster pruning, and discriminant function analysis (DFA) ;
# set up 1 Nov 09, for NB data;
# For R versions >= 1.9.0;
##################################################;
require(vegan)
require(cluster)
require(MASS) # Loads the library
require(gtools)
require(clValid)
########################;

##Loads the cal data, processes it, and saves it as a R binary file
##This function only needs to be run when creating (or changing the RCA model)
save_cal<-function(hab_data_file='cal_hab.csv',sp_data_file='cal_sp.csv',rca_model='atlantic_rca_model.RData'){

  hab_data<-read.csv(hab_data_file,header=TRUE)
  sp_data<-read.csv(sp_data_file,header=TRUE)

  cal<-process_cal_data(hab_data,sp_data)
  
  save(cal,file=rca_model)

}

##Runs test data and compares it to the saved RCA model
run_test_rca<-function(hab_data,sp_data,outfile_pa,outfile_ra,rca_model='atlantic_rca_model.RData'){

  #load "cal" RCA model object
  load(rca_model)
		
  #import and prepare test data
  test<-process_test_data(hab_data,sp_data)

  #Run presence absence RCA
  OE.assess.test<-model.predict.v4.1(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,
                                     prednew=test$predtest,bugnew=test$bioltest.pa,Pc=0.5)

  #Run relative abundnace RCA
  OE.assess.test.ra<-model.predict.v4.2(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,
                                        prednew=test$predtest,bugnew.pa=test$bioltest.pa,Pc=0.5,bugcal.ra=cal$bugcal,bugnew.ra=test$bioltest)
  
  #Output presence/absence RCA to file
  write.csv(OE.assess.test$OE.scores, file=outfile_pa)
        	
  write.csv(OE.assess.test.ra$OE.scores, file=outfile_ra)

  #return the two RCA runs
  list(OE.assess.test=OE.assess.test,OE.assess.test.ra=OE.assess.test.ra)

}

##This can probably be removed
##Dumping ground for some old code
run_rca<-function(){

  Data <- read.csv("D:/1. David/UNB_CRI/NB_Habitat_Project/2. Relative Abundance - RIVPACS/proAnalysis/Bio_GIS_AtlanticCanada_NewData.csv", header =TRUE, row.names=1) 

  hab_data<-read.csv("extras/RCA_locs_Vince.csv")
  sp_data<-read.csv("extras/")

  cal<-process_cal_data(hab_data,sp_data)
  
  save(cal,"atlantic_rca_model.RData")

  #list(bucal.pa=biolcal.pa,grps.final=grps.final,preds.final=preds.final, grpmns=grpmns,covpinv=covpinv,predcal=predcal,bugcal=biolcal)

#Option 5.1 - Make predictions of E and O/E for calibration (reference) sites. Examine O/E statistics and plots;
  # To do this, run the model.predict function, using the calibration data as the 'new' data;
  # See Step 7 below, for more info on making predictions, and also ;
  # see internal documentation of model.predict.v4.1;

OE.assess.cal<-model.predict.v4.1(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,prednew=cal$predcal,bugnew=cal$bugcal.pa,Pc=0.5)
OE.assess.cal.ra<-model.predict.v4.2(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,prednew=cal$predcal,bugnew.pa=cal$bugcal.pa,Pc=0.5,
                                   bugcal.ra=cal$bugcal,bugnew.ra=cal$bugcal);

write.csv(OE.assess.cal$OE.scores, file="OE_cal.csv")
write.csv(OE.assess.cal.ra$OE.scores, file="OE_cal_ra.csv")
  
     
  TestDataset <- read.csv("testall.csv", header =TRUE, row.names=1)    # Should include samples in the rows and environmental variables as first columns and biological variables as last ones
  colnames(TestDataset)
TestDataset[is.na(TestDataset)] <- 0  # insert 0 for missing value WE SHOULD CHANGE THIS

  test<-process_test_data(hab_data=TestDataset[1:4],sp_data=TestDataset[5:ncol(TestDataset)])
  #list(bioltest.pa=bioltest.pa,bioltest=bioltest,predtest=predtest)

###Repeat Step 5.1, but this time predict for test data.;
#pass biolcal and bioltest (these are the RA versions of the matrices)
OE.assess.test<-model.predict.v4.1(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,prednew=test$predtest,bugnew=test$bioltest.pa,Pc=0.5)
OE.assess.test.ra<-model.predict.v4.2(bugcal.pa=cal$bugcal.pa,cal$grps.final,cal$preds.final, cal$grpmns,cal$covpinv,
                                      prednew=test$predtest,bugnew.pa=test$bioltest.pa,Pc=0.5,bugcal.ra=cal$bugcal,bugnew.ra=test$bioltest)
write.csv(OE.assess.test$OE.scores, file="D:/1. David/UNB_CRI/NB_Habitat_Project/2. Relative Abundance - RIVPACS/proAnalysis/OE_test.csv")
write.csv(OE.assess.test.ra$OE.scores, file="D:/1. David/UNB_CRI/NB_Habitat_Project/2. Relative Abundance - RIVPACS/proAnalysis/OE_test_ra.csv")

}


process_cal_data<-function(hab_data,sp_data){

############;
# STEP 1 -- INITIAL SETUP -- Input and organize the biological and predictor data;
# The exact code for step is very particular for each data set, but similar operations will need;
# to be done on every data set;
# Data setup below is one example that was used for building NB models.
###############################;
# Input data are predictor data (all sites) and a (site x taxa) matrix for all taxa at all sites;
# Assume that predictor data file includes a column ("Subset")to ID the calibration ("c"), validation ("v") and test sites ("t");
# Input the predictor file, tab delimited;


# insert 0 for missing value WE SHOULD CHANGE THIS
  hab_data[is.na(hab_data)]<-0
  sp_data[is.na(sp_data)]<-0

  data<-process_data(hab_data,sp_data)
  
  predcal<-data$pred
  biolcal<-data$biol
  biolcal.pa<-data$biol.pa

colnames(biolcal)    # please check headers of biological matrix
colnames(predcal)   # please check headers of environmental matrix
rownames(biolcal)  # please check headers of sample matrix
#check sample(row) alignment of bug and predictor data;
row.names(biolcal)==row.names(predcal);


########################################;
#STEP 2 -- DISSIMILARITIES AND CLUSTER ANALYSIS;

#################################;
# compute dissimilarity matrix for calibration site bugs;
# The code below calcuates Bray-Curtis dissimilarities;

#########################################;
#Option 3 -- Bray-Curtis dissimilarity for abundance data;
#Proceed to clustering;
#########################;
####################################;
# Clustering of calibration sites;
# Ward method was selected
# Use breaks in slope of the agglomeration coefficient;
# Method can be obtained in agnes() function of "cluster" package;

braycurtis <- vegdist(biolcal, method="bray")             # , upper=TRUE , diag=TRUE
clustering <- agnes(braycurtis, diss=TRUE, method = "average") #or ward?
grps <-cutree(as.hclust(clustering), k=3)
factorgrp<- as.factor(grps)

################################;

#Option 2 -- All subsets DFA; 

# Feasible for up to about 15 candidate predictors;
# User specifies a small number of best models for selected model orders;
# Wilks lambda, classification accuracy, and statistics of O/E are reported for each best model;
# If user supplies an independent set of validation data (bug data and predictor data), then;
# O/E statistics also computed for validation set;
# In addition (version 4), the CV confusion matrices are reported for each of the best models;
#Also, the null model statistics are available;

#specify a vector describing how many models of each order to keep; 
# The following example is for a case with 14 candidate predictors;
# For example, keep 5 models each for orders 1,2, ...13,
# and also keep the single (saturated) model of order 14,
candvar <-c("geo_sed","geo_intr","geo_vol", "geo_sedvol", "clim_Trange","top_area","top_slope","clim_Prec") 
numkeep<-c(rep(5,8))
preds.final<-c("clim_Trange", "geo_intr", "geo_sed", "geo_sedvol")

#STEP 4 - Finalize calculations of final, chosen predictive model;
# To specify the entire final model, you need to store/export 5 things:
# 4.1) The site-by-taxa matrix of observed presence/absence at calibration sites (bugcal.pa, already available);
# 4.2) Specify the vector of final group membership assignments at calibration sites(grps.final);
      grps.final<-grps
# 4.3) Specify the final selected predictor variables (preds.final) - already done in step 3.5;
# 4.4) Calculate the matrix(grpmns) of mean values of the preds.final variables for the final calibration site groups ;
     datmat<-as.matrix(predcal[,preds.final])
     grpmns<-apply(datmat,2,function(x)tapply(x,grps.final,mean));
# 4.5) Calculate the inverse pooled covariance matrix(covpinv) of the preds.final variables at the calibration sites;
     #First, calculate a list of covariance matrices for each group;
      covlist<-lapply(split.data.frame(datmat,grps.final),cov);
      #pooled cov matrix is weighted average of group matrices, weighted by group size. Johnson & Wichern, 11-64;
      grpsiz<-table(grps.final);
      ngrps<-length(grpsiz);
      npreds<-length(preds.final);
     #zero out an initial matrix for pooled covariance;
     covpool<-matrix(rep(0,npreds*npreds),nrow=npreds,dimnames=dimnames(covlist[[1]]));
     #weighted sum of covariance matrices;
     for(i in 1:ngrps){covpool<-covpool+(grpsiz[i]-1)*covlist[[i]]};
     covpool<-covpool/(sum(grpsiz)-ngrps);#renormalize;
     covpinv<-solve(covpool); #inverse of pooled cov matrix;

     #return variables we care about
     list(bugcal.pa=biolcal.pa,grps.final=grps.final,preds.final=preds.final, grpmns=grpmns,covpinv=covpinv,predcal=predcal,bugcal=biolcal)

}

process_data<-function(hab_data,sp_data){

  #log transformation of continuos numerical variables, please adjust the number of column ("clim_Trange") 
  log.trans <- log(hab_data["clim_Trange"]+1,10)
  
  #arcsinesqroot transformation of % variables, please adjust the number of column ("geo_intr", "geo_sed", "geo_sedvol")
  arcsinesqroot.trans <- asin(sqrt(hab_data[c("geo_intr", "geo_sed", "geo_sedvol")]))

  pred <- cbind(log.trans,arcsinesqroot.trans)
  
  biol <- decostand(sp_data[4:ncol(sp_data)], "total")
  biol.pa <- decostand(sp_data[4:ncol(sp_data)],"pa")

  list(biol.pa=biol.pa,biol=biol,pred=pred)  
}

process_test_data<-function(hab_data,sp_data){
  test<-process_data(hab_data,sp_data)
  list(bioltest.pa=test$biol.pa,bioltest=test$biol,predtest=test$pred)
}

model.predict.v4.2<-function(bugcal.pa,grps.final,preds.final,grpmns,covpinv,prednew,bugnew.pa,Pc,bugcal.ra,bugnew.ra) {;

#1. - initial definitions (for groups);
names(grps.final)<-row.names(bugcal.pa);
nsite.cal<-length(grps.final); #number of calibration sites;
npreds<-length(preds.final); #number of predictor variables;
grpsiz<-table(grps.final); #tabulate sites per group;
ngrps<-length(grpsiz);  #number of groups;

#2. Alignment of new predictor and bug data with model data;
   #2a) Align the rows (samples) of the new bug data to the new predictor data;
    bugnew.pa<-bugnew.pa[row.names(prednew),];
   #2b) reshape bugnew columns (taxa) to match those in bugcal.pa, and be in same order;
   # New bug data might have fewer or more columns (taxa) than calibration bug data;
   # create a new empty site x taxa matrix with bugcal.pa columns and bugnew rows, fill it with zeros;
   nsite.new<-dim(bugnew.pa)[[1]];
   ntaxa<-dim(bugcal.pa)[[2]];
   bugnew.pa<-matrix(rep(0,nsite.new*ntaxa),nrow=nsite.new,dimnames=list(rownames(bugnew.pa),names(bugcal.pa)));
   #loop through columns of new matrix and fill with columns of the original test data matrix;
   col.match<-match(dimnames(bugnew.pa)[[2]],dimnames(bugnew.pa)[[2]]);
   for(kcol in 1:ntaxa) if(!is.na(col.match[kcol]))bugnew.pa[,kcol]<-bugnew.pa[,col.match[kcol]];
 ################;

## 3. -- predict the group (cluster) membership for all new sites. ;
# Follow RIVPACS assumption of weighting ;
# the membership probabilities by Calibration group size, as a prior probability;
# Also, flag any outlier sites, using the chi-squared statistic;
#Store probs in matrix, sites are rows, columns are groups;
#Uses mahalanobis function, where new vector is taken as the 'center', mu,;
#and matrix of means is taken as the 'data matrix', x;

dmat<-as.matrix(prednew[,preds.final]); #matrix of predictor data for new samples, include only the predictor variables;

#3.1 -- compute the critical chi-squared values for flagging outlier samples;
# df = the MINIMUM of (a)(number of groups-1), and (b) number of predictor variables;
# will flag each sample at P-value =.05 and also P-value =.01 level;
dff<-min(c(npreds,(ngrps-1)));
crit.01<-qchisq(0.99,df=dff);
crit.05<-qchisq(0.95,df=dff);

# 3.2 - construct empty matrix for predicted membership probabilities;
nsit.new<-dim(dmat)[[1]]; #number of new samples;
grpprobs<-matrix(rep(0,nsit.new*ngrps),nrow=nsit.new,
            dimnames=list(dimnames(dmat)[[1]],dimnames(grpmns)[[1]]));

#Also construct empty data.frame for outlier flag;
# include minimum (squared)distance;
# Each sample is either a PASS (denote by 0) or FAIL (denote by 1) for the outlier test;
 outlier.flag<-data.frame(outlier.05=rep(0,nsit.new),outlier.01=rep(0,nsit.new),dismin=rep(0,nsit.new),row.names=dimnames(dmat)[[1]]);

# 3.3 - Loop over ALL samples, compute vector of group membership probs for each sample and set its outlier flags;
#execute the following code piece as a single block;
##;
for(i in 1:nsit.new){;
#vector of squared Mahal. dist from current sample to each group mean;
dist<-mahalanobis(grpmns,dmat[i,],covpinv,inverted=T); #vector of distances;
grpprobs[i,]<-grpsiz*exp(-0.5*dist); # see Clarke et al. (2000);
grpprobs[i,]<-grpprobs[i,]/sum(grpprobs[i,]);
#check for outlier;
outlier.flag$dismin[i]<-min(dist); #save minimum distance;
if(outlier.flag$dismin[i]>crit.05)outlier.flag[i,'outlier.05']<-1;
if(outlier.flag$dismin[i]>crit.01)outlier.flag[i,'outlier.01']<-1;
        }; #finish sample loop;

## Shannon
H.cal.grp1 <- mean(diversity(subset(bugcal.ra, grps.final == 1)))
H.cal.grp2 <- mean(diversity(subset(bugcal.ra, grps.final == 2)))
H.cal.grp3 <- mean(diversity(subset(bugcal.ra, grps.final == 3)))
w.avg.grps.metrics.H <- cbind(H.cal.grp1, H.cal.grp2,H.cal.grp3)
#print(w.avg.grps.metrics.H)

## Simpson
S.cal.grp1 <- mean(diversity(subset(bugcal.ra, grps.final == 1), "simpson"))
S.cal.grp2 <- mean(diversity(subset(bugcal.ra, grps.final == 2), "simpson"))
S.cal.grp3 <- mean(diversity(subset(bugcal.ra, grps.final == 3), "simpson"))
w.avg.grps.metrics.S <- cbind(S.cal.grp1, S.cal.grp2,S.cal.grp3)
#print(w.avg.grps.metrics.S)

## Species richness (S)
T.cal.grp1 <- specnumber(subset(bugcal.ra, grps.final==1))
T.cal.grp2 <- specnumber(subset(bugcal.ra, grps.final==2))
T.cal.grp3 <- specnumber(subset(bugcal.ra, grps.final==3))


J.cal.grp1 <- mean(H.cal.grp1/log(T.cal.grp1))
J.cal.grp2 <- mean(H.cal.grp2/log(T.cal.grp2))
J.cal.grp3 <- mean(H.cal.grp3/log(T.cal.grp3))
w.avg.grps.metrics.J <- cbind(J.cal.grp1, J.cal.grp2,J.cal.grp3)
#print(w.avg.grps.metrics.J)

D.cal.grp1 <- mean(1-(apply(subset(bugcal.ra, grps.final == 1),1,max)))
D.cal.grp2 <- mean(1-(apply(subset(bugcal.ra, grps.final == 2),1,max)))
D.cal.grp3 <- mean(1-(apply(subset(bugcal.ra, grps.final == 3),1,max)))
w.avg.grps.metrics.D <- cbind(D.cal.grp1, D.cal.grp2,D.cal.grp3)
#print(w.avg.grps.metrics.D)



                                                  #row.names=row.names(prednew)
OE.stats<-data.frame(E.prd=rep(NA,nsit.new));
for(i in 1:nsit.new) {;
cur.obs.ra<-bugnew.ra[i,]
OE.stats$O.H[i]<-    diversity(cur.obs.ra)
OE.stats$E.H[i]<-    (grpprobs[i,1]%*%w.avg.grps.metrics.H[,1])+(grpprobs[i,2]%*%w.avg.grps.metrics.H[,2])+(grpprobs[i,3]%*%w.avg.grps.metrics.H[,3])

OE.stats$O.S[i]<-     diversity(cur.obs.ra, "simpson")
OE.stats$E.S[i]<-    (grpprobs[i,1]%*%w.avg.grps.metrics.S[,1])+(grpprobs[i,2]%*%w.avg.grps.metrics.S[,2])+(grpprobs[i,3]%*%w.avg.grps.metrics.S[,3])

OE.stats$O.J[i]<-     diversity(cur.obs.ra)/log(specnumber(cur.obs.ra))
OE.stats$E.J[i]<-    (grpprobs[i,1]%*%w.avg.grps.metrics.J[,1])+(grpprobs[i,2]%*%w.avg.grps.metrics.J[,2])+(grpprobs[i,3]%*%w.avg.grps.metrics.J[,3])

OE.stats$O.D[i]<-     1-(apply(cur.obs.ra,1,max))
OE.stats$E.D[i]<-    (grpprobs[i,1]%*%w.avg.grps.metrics.D[,1])+(grpprobs[i,2]%*%w.avg.grps.metrics.D[,2])+(grpprobs[i,3]%*%w.avg.grps.metrics.D[,3])

Enull.H<-mean(diversity(bugcal.ra));
#Obsnull.H[i]<- diversity(cur.obs.ra);

Enull.S<-mean(diversity(bugcal.ra, "simpson"));
#Obsnull.S[i]<- diversity(cur.obs.ra, "simpson");

Enull.J<-mean(diversity(bugcal.ra)/log(specnumber(bugcal.ra)));
#Obsnull.J[i]<- diversity(cur.obs.ra)/log(specnumber(cur.obs.ra));

Enull.D<-mean(1-(apply(bugcal.ra,1,max)));
#Obsnull.D[i]<- max(cur.obs.ra);
};

#5.2 - Compute Expected richness (E) and Observed metric value for null model



OE.final<-data.frame(
O.H=OE.stats$O.H,E.H=OE.stats$E.H, OoverE.H=OE.stats$O.H/OE.stats$E.H,
O.S=OE.stats$O.S,E.S=OE.stats$E.S, OoverE.S=OE.stats$O.S/OE.stats$E.S,
O.J=OE.stats$O.J,E.J=OE.stats$E.J, OoverE.J=OE.stats$O.J/OE.stats$E.J,
O.D=OE.stats$O.D,E.D=OE.stats$E.D, OoverE.D=OE.stats$O.D/OE.stats$E.D,

Onull.H=OE.stats$O.H,Enull.H=rep(Enull.H,length(OE.stats$O.H)),OoverE.null.H=OE.stats$O.H/Enull.H,
Onull.S=OE.stats$O.S,Enull.S=rep(Enull.S,length(OE.stats$O.S)),OoverE.null.S=OE.stats$O.S/Enull.S,
Onull.J=OE.stats$O.J,Enull.J=rep(Enull.J,length(OE.stats$O.J)),OoverE.null.J=OE.stats$O.J/Enull.J,
Onull.D=OE.stats$O.D,Enull.D=rep(Enull.D,length(OE.stats$O.D)),OoverE.null.D=OE.stats$O.D/Enull.D,

outlier.05=outlier.flag$outlier.05,outlier.01=outlier.flag$outlier.01,

   row.names=row.names(bugnew.ra));
#function output is a list containing OE.final, matrix of predicted capture probs, and predicted group membership probs
  list(OE.scores=OE.final)
}
                                                                                                                      
                                                                                                                     


model.predict.v4.1<-function(bugcal.pa,grps.final,preds.final,grpmns,covpinv,prednew,bugnew,Pc) {;

#first convert bug matrix to P/A (1/0);
   temp.pa<-bugnew;
   temp.pa[temp.pa>0]<-1;
   rm(bugnew);

#1. - initial definitions (for groups);
names(grps.final)<-row.names(bugcal.pa);
nsite.cal<-length(grps.final); #number of calibration sites;
npreds<-length(preds.final); #number of predictor variables;
grpsiz<-table(grps.final); #tabulate sites per group;
ngrps<-length(grpsiz);  #number of groups;

#2. Alignment of new predictor and bug data with model data;
   #2a) Align the rows (samples) of the new bug data to the new predictor data;
    temp.pa<-temp.pa[row.names(prednew),];
   #2b) reshape bugnew columns (taxa) to match those in bugcal.pa, and be in same order;
   # New bug data might have fewer or more columns (taxa) than calibration bug data;
   # create a new empty site x taxa matrix with bugcal.pa columns and bugnew rows, fill it with zeros;
   nsite.new<-dim(temp.pa)[[1]];
   ntaxa<-dim(bugcal.pa)[[2]];
   bugnew.pa<-matrix(rep(0,nsite.new*ntaxa),nrow=nsite.new,dimnames=list(rownames(temp.pa),names(bugcal.pa)));
   #loop through columns of new matrix and fill with columns of the original test data matrix;
   col.match<-match(dimnames(bugnew.pa)[[2]],dimnames(temp.pa)[[2]]);
   for(kcol in 1:ntaxa) if(!is.na(col.match[kcol]))bugnew.pa[,kcol]<-temp.pa[,col.match[kcol]];
 ################;

## 3. -- predict the group (cluster) membership for all new sites. ;
# Follow RIVPACS assumption of weighting ;
# the membership probabilities by Calibration group size, as a prior probability;
# Also, flag any outlier sites, using the chi-squared statistic;
#Store probs in matrix, sites are rows, columns are groups;
#Uses mahalanobis function, where new vector is taken as the 'center', mu,;
#and matrix of means is taken as the 'data matrix', x;

dmat<-as.matrix(prednew[,preds.final]); #matrix of predictor data for new samples, include only the predictor variables;

#3.1 -- compute the critical chi-squared values for flagging outlier samples;
# df = the MINIMUM of (a)(number of groups-1), and (b) number of predictor variables;
# will flag each sample at P-value =.05 and also P-value =.01 level;
dff<-min(c(npreds,(ngrps-1)));
crit.01<-qchisq(0.99,df=dff);
crit.05<-qchisq(0.95,df=dff);

# 3.2 - construct empty matrix for predicted membership probabilities;
nsit.new<-dim(dmat)[[1]]; #number of new samples;
grpprobs<-matrix(rep(0,nsit.new*ngrps),nrow=nsit.new,
            dimnames=list(dimnames(dmat)[[1]],dimnames(grpmns)[[1]]));

#Also construct empty data.frame for outlier flag;
# include minimum (squared)distance;
# Each sample is either a PASS (denote by 0) or FAIL (denote by 1) for the outlier test;
 outlier.flag<-data.frame(outlier.05=rep(0,nsit.new),outlier.01=rep(0,nsit.new),dismin=rep(0,nsit.new),row.names=dimnames(dmat)[[1]]);

# 3.3 - Loop over ALL samples, compute vector of group membership probs for each sample and set its outlier flags;
#execute the following code piece as a single block;
##;
for(i in 1:nsit.new){;
#vector of squared Mahal. dist from current sample to each group mean;
dist<-mahalanobis(grpmns,dmat[i,],covpinv,inverted=T); #vector of distances;
grpprobs[i,]<-grpsiz*exp(-0.5*dist); # see Clarke et al. (2000);
grpprobs[i,]<-grpprobs[i,]/sum(grpprobs[i,]);
#check for outlier;
outlier.flag$dismin[i]<-min(dist); #save minimum distance;
if(outlier.flag$dismin[i]>crit.05)outlier.flag[i,'outlier.05']<-1;
if(outlier.flag$dismin[i]>crit.01)outlier.flag[i,'outlier.01']<-1;
        }; #finish sample loop;
#### sample membership probabilities complete for new samples;

#########MORGAN: grpprobs[] contains group membership probabilities
                                                                                                 
                                                                                                 
############;
#STEP 4 -- Compute predicted occurrence probability for each modeled taxon at each new sample;
# "modeled taxa" consist of all taxa that were found at >=1 calibration sample;
#To do this, first calculate the occurrence freqs of all modeled taxa in the Calibration sample groups;
grpocc<-apply(bugcal.pa,2,function(x){tapply(x,grps.final,function(y){sum(y)/length(y)})});

#finally, compute the matrix of predicted occurrence (capture) probabilities, for all new samples and all modeled taxa;
#This is the matrix-algebra form of the RIVPACS combining formula (e.g., Clarke et al. 2003, Eq. 4)
site.pred.dfa<-grpprobs%*%grpocc;

#########MORGAN: grpocc is the taxon occurence prob matrix
#########MORGAN: site.pred.dfa contains the taxon occurence probablity matrix for each site weighted for group membership (ones above >0.5 are kept)

##STEP 4B (NEW)
##Do same as above but calculate grpocc using relative abundance (RA) reference (Expected) matrix
##grpocc is average relative abundance (which might be same as above (look into more detail)
##create a new site.pred.dfa for RA using same grpprobs                                                                                                 
##Call this site.pred.ra.dfa                                                                                                                                                                                                
#######################;
###NEED TO ALTER THIS FOR RELATIVE ABUNDANCE
                                                                                                 
# STEP 5. Compute O, E, O/E and BC for all samples. ;
# Also compute O/E and BC for the null model;

#5.1 loop over all samples. Compute and store  O, predicted E, predicted BC for each sample. ;
#temporary data frame to hold nonnull results for all samples. ;
OE.stats<-data.frame(OBS=rep(NA,nsit.new), E.prd=rep(NA,nsit.new),BC.prd=rep(NA,nsit.new),row.names=row.names(prednew));
for(i in 1:nsit.new) {;
   #i<-1;
   cur.prd<-site.pred.dfa[i,]; #vector of predicted probs for current sample;
   spdyn<-names(cur.prd)[cur.prd>=Pc];  #subset of taxa with Pi>=Pcutoff; #MORGAN get names of ones above cutoff
   cur.prd<-cur.prd[spdyn]; #vector of Pi for subset of included taxa; #MORGAN keep those above cutoff for expected list:CHANGE to apply to E RA matrix
   cur.obs<-bugnew.pa[i,spdyn]; #vector of observed P/A for those taxa; #MORGAN keep those above cutoff for observed list: CHANGE to apply to O RA Matrix

   ##Keep these simple stats
   OE.stats$OBS[i]<-sum(cur.obs); #observed richness (O);
   OE.stats$E.prd[i]<-sum(cur.prd); #Expected richness (E);
   OE.stats$BC.prd[i]<-sum(abs(cur.obs-cur.prd))/ (OE.stats$OBS[i]+OE.stats$E.prd[i]); #BC value;

   ##Add in RA stats (Change following stats to use cur.prd and cur.obs for RA)
  # Biological metrics computation - calibration
                      H.cal <- diversity(bugcal.pa)  # Shannon or Shannon–Weaver (or Shannon–Wiener) index H
                      S.cal <- diversity(bugcal.pa, "simpson")  #Simpson index S
                      T.cal <- specnumber(bugcal.pa) ## ## Species richness (S)
                      J.cal <- H.cal/log(T.cal) ## Pielou's evenness (J)
                      D.cal <- apply(bugcal.pa, 1, max)
                      
                    }; #finish sample loop;

#5.2 - Compute Expected richness (E) and BC for null model using taxa >= Pc.
# Note that the set of taxa included in the null model is fixed for all samples;
pnull<-apply(bugcal.pa,2,sum)/dim(bugcal.pa)[[1]];  #null model predicted occurrnece probabilities, all taxa;
nulltax<-names(pnull[pnull>=Pc]); #subset of taxa with Pnull >= Pc;
Enull<-sum(pnull[nulltax]);
Obsnull<-apply(bugnew.pa[,nulltax],1,sum); #vector of Observed richness, new samples, under null model;
BC.null<-apply(bugnew.pa[,nulltax],1,function(x)sum(abs(x-pnull[nulltax])))/(Obsnull+Enull); #vector of null-model BC;

#5.3 - Final data frame contains values of O, E, O/E, Onull, Enull, Onull/Enull, BC.prd and BC.null, for all samples;
#Also includes outlier flags;


OE.final<-data.frame(O=OE.stats$OBS,E=OE.stats$E.prd,
                      OoverE=OE.stats$OBS/OE.stats$E.prd, 
   Onull=Obsnull,Enull=rep(Enull,length(Obsnull)),OoverE.null=Obsnull/Enull,
   BC= OE.stats$BC.prd,BC.null=BC.null,
     outlier.05=outlier.flag$outlier.05,outlier.01=outlier.flag$outlier.01,
                    row.names=row.names(bugnew.pa));
###########;
#print some summary statistics of O/E to wrap up;
#print(' ',quote=F)
#print('Statistics of O/E for new samples',quote=F);
#print(' ',quote=F)
#  c1<-mean(OE.final$OoverE); c2<-mean(OE.final$OoverE.null);
#  s1<-sqrt(var(OE.final$OoverE)); s2<-sqrt(var(OE.final$OoverE.null));
#print(' Mean(O/E) and SD(O/E), from predictive model: ',quote=F)
#print( c(c1,s1),digits=3);
#print(' ',quote=F)
#print(' Mean(O/E) and SD(O/E), from null model: ',quote=F)
#print( c(c2,s2),digits=3);
#print outlier count;
#print(' ',quote=F)
#print('Counts of OK (=0) and outlier (=1) samples, assessed at P=0.01 level of chi-square',quote=F);
#print(table(OE.final[,'outlier.01']));
#print(' ',quote=F)
#print('Predictions complete', quote=F);

#function output is a list containing OE.final, matrix of predicted capture probs, and predicted group membership probs;
list(OE.scores=OE.final,Capture.Probs=site.pred.dfa,Group.Occurrence.Probs=grpprobs)
} #model.predict.v4.1;


example_run_test_rca<-function(){

  data <- read.csv("testall.csv", header =TRUE, row.names=1)  
  
  data[is.na(data)] <- 0  # insert 0 for missing value WE SHOULD CHANGE THIS


  hab_data<-data[c("clim_Trange","geo_intr", "geo_sed", "geo_sedvol")]

  sp_data<-data[7:ncol(data)]

  run_test_rca(hab_data,sp_data,'test_rca_out_pa.txt','test_rca_out_ra.txt')


}

##MAIN
##Any functions uncommented below will cause this file to act as a script. 
##Except during testing all statements below should be commented out.
 

##to activate debugger place problematic function in debug statement before running the function
#debug(run_test_rca)

##Create and save the RCA model
#save_cal()

##use test data to run rca model
#example_run_test_rca()
