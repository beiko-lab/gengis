import GenGIS

class HelloWorld():
	def __init__(self, parent=None):
		label = GenGIS.VisualLabel("Hello World!", GenGIS.Colour(0,0,0), 36, GenGIS.LABEL_RENDERING_STYLE.ORTHO)
		label.SetScreenPosition(GenGIS.Point3D(10,40,1))
		GenGIS.graphics.AddLabel(label)
		GenGIS.viewport.Refresh()