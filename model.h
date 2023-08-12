#ifndef H_MODEL
#define H_MODEL

#include "hammer.h"

/* Counts number of triangles in GLfloat array */
#define vsize(x) \
	( ( sizeof(x) / sizeof(GLfloat) ) / 3 )

typedef struct h_Model h_Model;

struct h_Model {

	/* Vertex Buffer Object, Index Buffer Object */
	GLuint VBO, EBO;
	int shaderID;

	/* Coordinates of vertices, textures and normals */
	GLfloat v[65500], vt[65500], vn[65500];

	/* Indices for vertices, textures and normals */
	GLushort f_v[65500], f_vt[65500], f_vn[65500];
	
	/* Size in bytes */
	size_t v_size, vt_size, vn_size, f_v_size, f_vt_size, f_vn_size;

	/* Exact number of elements, for example, one v = 3 etc... */
	int v_count, vt_count, vn_count, f_v_count, f_vt_count, f_vn_count;

	/* Use it with vsize() macro, model.size = vsize(model.data) */
	int size;

	/* Used for rendering mode, GL_TRIANGLES is default and renders is normally,
	 * GL_LINES renders only lines, GL_POINTS only points etc... */
	GLenum render_mode;

	/* Geometry information */
	mat4 m4Projection, m4View, m4Model, m4MVP;
	float fAngle, fAspectRatio, fNear, fFar;
	vec3 v3Origin, v3Camera, v3Up;

	char path[255];
	
	/* Function pointers, makes code a little bit nicer.
	 * Optional, you can use functions directly, i prefer OOP-like style.
	 */
	void (*Render)(h_Model *);
	void (*Update)(h_Model *);
	void (*Destroy)(h_Model *);
};

extern h_Model h_ModelInit(char *);
extern void h_ModelRender(h_Model *);
extern void h_ModelUpdate(h_Model *);
extern GLfloat *h_ModelLoadModel(h_Model *, char *);
extern void h_ModelDestroy(h_Model *);

#endif
