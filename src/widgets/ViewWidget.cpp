//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "../core/Precompiled.hpp"

#include "../widgets/ViewWidget.hpp"
#include "../widgets/Texture.hpp"

#include "../core/App.hpp"
#include "../core/OrthoLayers.hpp"

using namespace GenGIS;

void ViewWidget::SetTexture( TexturePtr texture ) 
{
	m_texture = texture;
	m_textured = true;
}

void ViewWidget::SetScreenCoords(const Point3D& screenC ) 
{
	m_screenCoordinates.x = screenC.x;
	m_screenCoordinates.y = screenC.y;
	m_screenCoordinates.z = screenC.z;
}

Box2D ViewWidget::GetBoundingBox() const
{
	Box2D box;
	box.x = -m_scale.x + m_screenCoordinates.x;
	box.y = -m_scale.y + m_screenCoordinates.y;
	box.dx = 2 * m_scale.x;
	box.dy = 2 * m_scale.y;

	return box;
}

void ViewWidget::Render() 
{
	if(m_bVisible && !App::Inst().IsSavingHiResImage())
	{
		glPushMatrix();

			glTranslated( m_screenCoordinates.x, m_screenCoordinates.y, -WIDGETS_LAYER );
			glScaled( m_scale.x, m_scale.y, m_scale.z );
			glDisable( GL_LIGHTING );

			if ( m_textured ) 
			{
				glBindTexture(GL_TEXTURE_2D, m_texture->GetName());
				glEnable(GL_TEXTURE_2D);

				glColor4f( 1, 1, 1, 1 );
			  
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 1);
					glVertex2f(-1, -1);

					glTexCoord2f(1, 1);
					glVertex2f(+1, -1);

					glTexCoord2f(1, 0);
					glVertex2f(+1, +1);

					glTexCoord2f(0, 0);
					glVertex2f(-1, +1);
				glEnd();

				glDisable(GL_TEXTURE_2D);
			}

			glEnable( GL_LIGHTING );

		glPopMatrix();
	}
}
