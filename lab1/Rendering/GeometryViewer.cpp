#include "Rendering/GeometryViewer.h"
#include "Common/TinyGeom.h" 
#include <FL/gl.h> 
#include <GL/glu.h>

#include <iostream>
using namespace std; 

extern "C"{
#include "Common/bmpfile.h"
}

using namespace TinyGeom; 

GeometryViewer::GeometryViewer(int x, int y, int w, int h, const char* l)
: Fl_Gl_Window(x,y,w,h,l){
	Fl::repeat_timeout(REFRESH_RATE,GeometryViewer::updateCb,this);
	_w = w; 
	_h = h; 
	_selected = NULL; 
	_highlighted = NULL;  
	_selectedPt = NULL; 
	_highlightedPt = NULL; 
	_transgrid = NULL; 
	_geomhist.pushNew(); 
	this->border(5); 

	defaultView(); 
	_showGrid = true; 
}

GeometryViewer::~GeometryViewer(){
	Fl::remove_timeout(GeometryViewer::updateCb,this); 
}

void GeometryViewer::set2DProjection(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(_dspaceLL[0], _dspaceUR[0], _dspaceLL[1], _dspaceUR[1]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void GeometryViewer::draw(){
	if(!valid())
		init();
//	this->make_current(); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	set2DProjection(); // necessary for 2d drawing
	glDisable(GL_LIGHTING);

	glColor3f(1.f,1.f,1.f); 
	if(_transgrid && _showGrid){
		const list<pair<int,int> >& edges = _transgrid->getEdges(); 
		const vector<Pt2>& pts = _transgrid->getPts(); 

		glBegin(GL_LINES); 
		for(list<pair<int,int> >::const_iterator i=edges.begin(); i!=edges.end(); i++){
			glVertex2d(pts[i->first][0],pts[i->first][1]); 
			glVertex2d(pts[i->second][0],pts[i->second][1]); 
		}
		glEnd(); 
	}


	glColor3f(1.f,0.f,0.f); 
	for(list<pair<Geom2*,Color> >::iterator i=_geomhist.getTop()->begin();i!=_geomhist.getTop()->end();i++){
		Color c = i->second; 
		Geom2* g = i->first; 
		glColor3d(c[0],c[1],c[2]); 
		glBegin(GL_POLYGON); 
		for(int j=0;j<g->size();j++){
			Pt2* p = g->get(j); 
			glVertex2d((*p)[0],(*p)[1]); 
		}
		glEnd(); 

		if(_editing.find(g)!=_editing.end()){
			glLineWidth(3.f); 
			glColor3f(0.,1.,0); 
			glBegin(GL_LINE_LOOP); 
			for(int j=0;j<g->size();j++){
				Pt2* p = g->get(j); 
				glVertex2d((*p)[0],(*p)[1]); 
			}
			glEnd(); 
			glLineWidth(1.f); 
		}

		if(g==_highlighted){
			glLineWidth(2.f); 
			glColor3f(1.,0,0); 
			glBegin(GL_LINE_LOOP); 
			for(int j=0;j<g->size();j++){
				Pt2* p = g->get(j); 
				glVertex2d((*p)[0],(*p)[1]); 
			}
			glVertex2d((*g->get(0))[0],(*g->get(0))[1]); 
			glEnd(); 
			glLineWidth(1.f); 
		}

		for(int j=0;j<g->size();j++){
			Pt2* p = g->get(j); 
			if(p==_highlightedPt){
				glBegin(GL_POINTS); 
				glColor3f(1,0,0); 

				glVertex2d((*p)[0],(*p)[1]); 
				glEnd(); 
			}
		}
	}

	swap_buffers(); 
}

Pt2 GeometryViewer::win2Screen(int x, int y){
	Vec2 winv(x/(double)getWidth(),(getHeight()-y)/(double)getHeight(),0); 
	Vec2 diff = _dspaceUR-_dspaceLL; 
	diff[0]*=winv[0]; 
	diff[1]*=winv[1]; 

	return (_dspaceLL+diff); 
}

int GeometryViewer::handle(int ev){
	// input in 2d mode
	if(ev==FL_PUSH){
		if(Fl::event_button()==FL_LEFT_MOUSE){
			_prevpos = win2Screen(Fl::event_x(),Fl::event_y()); 
			if(_highlighted){
				_selected = _highlighted; 
			}
			else if(_highlightedPt){
				_selectedPt = _highlightedPt; 
			}
			else
				_panning = true; 
		}
		else if(Fl::event_button()==FL_RIGHT_MOUSE){
			if(!_highlighted && !_highlightedPt){
				_prevpos = Pt2(Fl::event_x(),Fl::event_y()); 
				_zooming = true; 
			}
		}
		return 1;  // must return 1 here to ensure FL_MOVE is sent
	}
	else if(ev==FL_DRAG){
		if(Fl::event_button()==FL_LEFT_MOUSE){
			Pt2 mpos = win2Screen(Fl::event_x(),Fl::event_y()); 
			if(_selected){
				Vec2 v = mpos - _prevpos; 
				for(int j=0;j<_selected->size();j++){
					(*_selected->get(j))[0]+=v[0]; 
					(*_selected->get(j))[1]+=v[1]; 
				}
				_prevpos = mpos; 
			}
			else if(_selectedPt){
				Geom2* g2 = _p2geom[_selectedPt]; 
				Quad2* quad = dynamic_cast<Quad2*>(g2); 
				Hex2* hex = dynamic_cast<Hex2*>(g2);
				Oct2* oct = dynamic_cast<Oct2*>(g2);
				Cir2* cir = dynamic_cast<Cir2*>(g2);
				Star2* star = dynamic_cast<Star2*>(g2);
				// TODO: add more shapes
				
				if(quad){
					Vec2 v = mpos - _prevpos;
					Pt2 prevp = (*_selectedPt); 
					(*_selectedPt)[0]+=v[0]; 
					(*_selectedPt)[1]+=v[1]; 

					int aind,bind,cind,dind; 
					for(int j=0;j<quad->size();j++){
						if
							
							
							(quad->get(j)==_selectedPt){
							aind = j; 
							break; 
						}
					}
					bind = (aind+1)%4; 
					cind = (aind+2)%4; 
					dind = (aind+3)%4; 

					Vec2 ba = (*quad->get(bind))-prevp; 
					Vec2 ca = (*quad->get(cind))-(*quad->get(aind)); 
					Vec2 ac = (*quad->get(aind))-(*quad->get(cind)); 
					Vec2 dc = (*quad->get(dind))-(*quad->get(cind)); 

					double len0 = ca*ba/mag(ba); 
					double len1 = ac*dc/mag(dc); 

					dc.normalize(); 
					ba.normalize(); 

					(*quad->get(dind)) = (*quad->get(cind))+(len1*dc); 
					(*quad->get(bind)) = (*quad->get(aind))+(len0*ba); 
				}
				else if (hex) {
					Vec2 v = mpos - _prevpos;
					Pt2 prevp = (*_selectedPt);// prevp: old selected point
					(*_selectedPt)[0] += v[0];
					(*_selectedPt)[1] += v[1];//selectedPt is new point

					int aind, bind, cind, dind, eind, gind;
					for (int j = 0; j < hex->size(); j++) {
						if (hex->get(j) == _selectedPt) {
							aind = j;
							break;
						}

					}
					Pt2 center_point(0,0,0);
					for (int i = 0; i < hex->size(); i++) {
						center_point = center_point + *hex->get(i);
					}

					center_point /= 6;

					Vec2 oa_old = prevp - center_point;
					Vec2 oa_new = *_selectedPt - center_point;
					


					double c_theta = (oa_old * oa_new) / (mag(oa_old) * mag(oa_new));

					double s_theta = mag((cross(oa_old, oa_new))) / (mag(oa_old) * mag(oa_new));
					if (cross(oa_old, oa_new)[2] < 0) {
						s_theta =  - s_theta;
					}

					double size = mag(oa_new) / mag(oa_old);

					for (int j = (aind + 1) % 6; j != aind; j = (j + 1) % 6) {
						Vec2 o_curpoint = (*hex->get(j)) - center_point;
						Vec2 oc_orth(-o_curpoint[1], o_curpoint[0], 0);
						Vec2 oc_new = (o_curpoint * c_theta + oc_orth * s_theta) * size;
						*hex->get(j) = oc_new + center_point;

					}

				}
				else if (oct) {

					Vec2 v = mpos - _prevpos;
					Pt2 prevp = (*_selectedPt);// prevp: old selected point
					(*_selectedPt)[0] += v[0];
					(*_selectedPt)[1] += v[1];//selectedPt is new point

					int aind, bind, cind, dind, eind, gind;
					for (int j = 0; j < oct->size(); j++) {
						if (oct->get(j) == _selectedPt) {
							aind = j;
							break;
						}

					}
					Pt2 center_point(0, 0, 0);
					for (int i = 0; i < oct->size(); i++) {
						center_point = center_point + *oct->get(i);
					}

					center_point /= 8;

					Vec2 oa_old = prevp - center_point;
					Vec2 oa_new = *_selectedPt - center_point;



					double c_theta = (oa_old * oa_new) / (mag(oa_old) * mag(oa_new));

					double s_theta = mag((cross(oa_old, oa_new))) / (mag(oa_old) * mag(oa_new));
					if (cross(oa_old, oa_new)[2] < 0) {
						s_theta = -s_theta;
					}

					double size = mag(oa_new) / mag(oa_old);

					for (int j = (aind + 1) % 8; j != aind; j = (j + 1) % 8) {
						Vec2 o_curpoint = (*oct->get(j)) - center_point;
						Vec2 oc_orth(-o_curpoint[1], o_curpoint[0], 0);
						Vec2 oc_new = (o_curpoint * c_theta + oc_orth * s_theta) * size;
						*oct->get(j) = oc_new + center_point;

					}
				}
				else if (cir) {
				Vec2 v = mpos - _prevpos;
				Pt2 prevp = (*_selectedPt);// prevp: old selected point
				(*_selectedPt)[0] += v[0];
				(*_selectedPt)[1] += v[1];//selectedPt is new point

				int aind, bind, cind, dind, eind, gind;
				for (int j = 0; j < cir->size(); j++) {
					if (cir->get(j) == _selectedPt) {
						aind = j;
						break;
					}

				}
				Pt2 center_point(0, 0, 0);
				for (int i = 0; i < cir->size(); i++) {
					center_point = center_point + *cir->get(i);
				}

				center_point /= 100;

				Vec2 oa_old = prevp - center_point;
				Vec2 oa_new = *_selectedPt - center_point;



				double c_theta = (oa_old * oa_new) / (mag(oa_old) * mag(oa_new));

				double s_theta = mag((cross(oa_old, oa_new))) / (mag(oa_old) * mag(oa_new));
				if (cross(oa_old, oa_new)[2] < 0) {
					s_theta = -s_theta;
				}

				double size = mag(oa_new) / mag(oa_old);

				for (int j = (aind + 1) % 100; j != aind; j = (j + 1) % 100) {
					Vec2 o_curpoint = (*cir->get(j)) - center_point;
					Vec2 oc_orth(-o_curpoint[1], o_curpoint[0], 0);
					Vec2 oc_new = (o_curpoint * c_theta + oc_orth * s_theta) * size;
					*cir->get(j) = oc_new + center_point;

				}

				}
				
				else if (star) {

				Vec2 v = mpos - _prevpos;
				Pt2 prevp = (*_selectedPt);// prevp: old selected point
				(*_selectedPt)[0] += v[0];
				(*_selectedPt)[1] += v[1];//selectedPt is new point

				int aind, bind, cind, dind, eind, gind;
				for (int j = 0; j < star->size(); j++) {
					if (star->get(j) == _selectedPt) {
						aind = j;
						break;
					}

				}
				Pt2 center_point(0, 0, 0);
				for (int i = 0; i < star->size(); i++) {
					center_point = center_point + *star->get(i);
				}

				center_point /= 12;

				Vec2 oa_old = prevp - center_point;
				Vec2 oa_new = *_selectedPt - center_point;



				double c_theta = (oa_old * oa_new) / (mag(oa_old) * mag(oa_new));

				double s_theta = mag((cross(oa_old, oa_new))) / (mag(oa_old) * mag(oa_new));
				if (cross(oa_old, oa_new)[2] < 0) {
					s_theta = -s_theta;
				}

				double size = mag(oa_new) / mag(oa_old);

				for (int j = (aind + 1) % 12; j != aind; j = (j + 1) % 12) {
					Vec2 o_curpoint = (*star->get(j)) - center_point;
					Vec2 oc_orth(-o_curpoint[1], o_curpoint[0], 0);
					Vec2 oc_new = (o_curpoint * c_theta + oc_orth * s_theta) * size;
					*star->get(j) = oc_new + center_point;

				}

				}

				else{
					// else it is a triangle.  
					Vec2 v = mpos - _prevpos;
					(*_selectedPt)+=v; 

					if(!Utils::isConvex(g2))
						(*_selectedPt)-=v; 
				}

				_prevpos = mpos; 
			}
			else if(_panning){
				Vec2 change = mpos-_prevpos; 
				_dspaceLL-=change; 
				_dspaceUR-=change; 
			}
		}
		else if(Fl::event_button()==FL_RIGHT_MOUSE){
			if(_zooming){
				Pt2 mpos(Fl::event_x(),Fl::event_y()); 
				double diffy = _prevpos[1]-mpos[1]; 
				Pt2 center = (_dspaceLL+_dspaceUR)*.5; 
				Vec2 lv = _dspaceLL-center; 
				Vec2 rv = _dspaceUR-center; 

				double fac = 1-(diffy)/500; 
				_dspaceLL = center+(fac*lv); 
				_dspaceUR = center+(fac*rv); 
				_prevpos = mpos; 
			}
		}
	}
	else if(ev==FL_RELEASE){
		if(Fl::event_button()==FL_RIGHT_MOUSE){
			if(_highlighted){
				if(_editing.find(_highlighted)==_editing.end()){
					_editing.insert(_highlighted); 
				}
				else{
					_editing.erase(_highlighted); 
				}
			}
		}

		_selected = NULL; 
		_highlighted = NULL; 
		_selectedPt = NULL; 
		_highlightedPt = NULL; 
		_panning = false; 
		_zooming = false; 
	}
	else if(ev==FL_MOVE){
		Pt2 mpos = win2Screen(Fl::event_x(),Fl::event_y()); 
		double ratio = Utils::dist2d(_dspaceLL,_dspaceUR)/600;

		// TODO: maybe need to modify for more shapes
		// check to see if the mouse is interior to some shape
		// isPtInterior only works for convex shapes.
		// if your shape is not-convex, you need to write a different function to check for interior-ness.
		_highlighted = NULL; 
		for(list<pair<Geom2*,Color> >::reverse_iterator i=_geomhist.getTop()->rbegin();i!=_geomhist.getTop()->rend();i++){
			if(Utils::isPtInterior(i->first,mpos)){
				_highlighted = i->first; 
				break; 
			}
		}

		_highlightedPt = NULL; 
		if(!_highlighted){
			double bestd=10000; 
			Pt2* best = NULL; 
			for(map<Pt2*,Geom2*>::iterator i=_p2geom.begin();i!=_p2geom.end();i++){
				double nd = Utils::dist2d(*(i->first),mpos); 
				if(nd<bestd){
					bestd = nd; 
					best = i->first; 
				}
			}
			if(best && bestd<5*ratio)
				_highlightedPt = best; 
		}
	}
	else if(ev==FL_KEYDOWN){}
	else if(ev==FL_KEYUP){}

	return Fl_Gl_Window::handle(ev);
}

void GeometryViewer::init(){
	glClearColor(0,0,0,1); 
	glMatrixMode(GL_PROJECTION); 
	glOrtho(0,this->w(),0,this->h(),-100,100); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glDisable(GL_DEPTH_TEST); 
	glLineWidth(1.f); 
	glPointSize(8.f); 
	glEnable(GL_POINT_SMOOTH); 
	glEnable(GL_POLYGON_SMOOTH); 

	set2DProjection(); 
}

void GeometryViewer::resize(int x, int y, int width, int height){
	make_current();
	_w = width; 
	_h = height; 
	Fl_Gl_Window::resize(x,y,width,height);
}

void GeometryViewer::addGeom(Geom2* geom){
	double r = rand()/(1.*RAND_MAX); 
	double g = rand()/(1.*RAND_MAX); 
	double b = rand()/(1.*RAND_MAX); 

	r=min(1,r+.2); 
	g=min(1,g+.2); 
	b=min(1,b+.2); 

	_geomhist.getTop()->push_back(make_pair(geom,Color(r,g,b))); 
	for(int j=0;j<geom->size();j++)
		_p2geom[geom->get(j)] = geom; 

	redraw(); 
}

void GeometryViewer::saveImageBufferCb(Fl_Widget* widget, void* userdata){
	GeometryViewer* viewer = (GeometryViewer*) userdata; 

	if(viewer){
		char* newfile = fl_file_chooser("Save image", ".bmp (*.bmp)", "./images", 0);
		if(!newfile) return; 

		int h = viewer->getHeight(); 
		int w = viewer->getWidth(); 
		viewer->make_current(); 

		GLubyte *data = new GLubyte[4*h*w]; 
		glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);

		bmpfile_t* bfile = bmp_create(w,h,32); 
		for(int j=0;j<h;j++){
			for(int i=0;i<w;i++){
				rgb_pixel_t pix = {data[(j*w+i)*4+2],data[(j*w+i)*4+1],data[(j*w+i)*4],data[(j*w+i)*4+3]}; 
				bmp_set_pixel(bfile,i,(h-1)-j,pix); 
			}
		}
		bmp_save(bfile,newfile); 
		bmp_destroy(bfile); 

		delete [] data; 

	}
}

void GeometryViewer::addShapeCb(Fl_Widget* widget,void* userdata){
	pair<GeometryViewer*,TGShape>* data = (pair<GeometryViewer*,TGShape>*) userdata; 
	GeometryViewer* viewer = data->first; 
	TGShape shape = data->second; 

	if(viewer){
		// randomly pick a point on the screen as the center of the shape
		int nw = viewer->getWidth() * .7; 
		int nh = viewer->getHeight() * .7; 

		int nx = rand()%nw - nw/2; 
		int ny = rand()%nh - nh/2; 

		// TODO: add code to account for more shapes
		Geom2* ng; 
		switch(shape){
          case TG_TRIANGLE: 
			ng = new Tri2(Pt2(nx,ny),50); 
			break; 
          case TG_QUAD: 
			ng = new Quad2(Pt2(nx,ny),50); 
			break;
		  case TG_HEX:
			ng = new Hex2(Pt2(nx, ny), 50);
			break;
		  
		  case TG_OCT:
			ng = new Oct2(Pt2(nx, ny), 50);
			break;
		  case TG_CIR:
			  ng = new Cir2(Pt2(nx, ny), 50);
			  break;
		   case TG_STAR:
		 	  ng = new Star2(Pt2(nx, ny), 50);
		 	  break;
			  
          default: 
			ng = new Tri2(Pt2(nx,ny),50); 
			break; 
		}
		viewer->addGeom(ng); 
	}
}

void GeometryViewer::undoCb(Fl_Widget* widget,void* userdata){
	GeometryViewer* viewer = (GeometryViewer*) userdata; 
	if(viewer){
		if(viewer->_geomhist.size()>1){
			viewer->_geomhist.popTop(); 
			if(viewer->_geomhist.getTop()==NULL)
				viewer->_geomhist.pushNew(); 
			viewer->prepareGeom(viewer->_geomhist.getTop()); 
		}
	}
}

void GeometryViewer::defaultViewCb(Fl_Widget*, void* userdata){
	GeometryViewer* viewer = (GeometryViewer*) userdata; 
	if(viewer)
		viewer->defaultView(); 
}


void GeometryViewer::delEditingShapesCb(Fl_Widget* widget,void* userdata){
	GeometryViewer* viewer = (GeometryViewer*) userdata; 
	if(viewer){
		list<pair<Geom2*,Color> >* geoms = viewer->_geomhist.getTop(); 
		list<pair<Geom2*,Color> > ngeoms; 

		for(list<pair<Geom2*,Color> >::iterator i=geoms->begin();i!=geoms->end();i++){
			if(viewer->_editing.find(i->first)==viewer->_editing.end())
				ngeoms.push_back(*i); 
		}
		(*geoms) = ngeoms; 

		for(set<Geom2*>::iterator i=viewer->_editing.begin();i!=viewer->_editing.end();i++)
			delete *i; 

		viewer->_editing.clear(); 
		viewer->prepareGeom(geoms); 
	}
}
