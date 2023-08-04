/* I do not plan to add support for any other model format, Wavefront's format
 * is good enough for me and it is easy to work with in my opinion.
 */

#include "model.h"

h_Model
h_ModelInit( char *path ){

	/* After every model loading this variable increments, thus
	 * ensuring original shader ID for every model. This also
	 * means that you can use unique shader for every model you load.
	 */
	static int shaderID;
	
	h_Model model = {
		.m4Projection = GLM_MAT4_IDENTITY_INIT,
		.m4View = GLM_MAT4_IDENTITY_INIT,
		.m4Model = GLM_MAT4_IDENTITY_INIT,
		.m4MVP = GLM_MAT4_IDENTITY_INIT,
		.shaderID = shaderID,
		.render_mode = GL_TRIANGLES,
		.Render = &h_ModelRender,
		.Update = &h_ModelUpdate,
		.Destroy = &h_ModelDestroy };

	strcpy(model.path, path);

	FILE *fp = fopen(path, "r");
	if(fp == NULL){
		printf("Model load error: %s\n", path);
	}

	char tmpc[100];

	int v_count = 0;
	int vt_count = 0;
	int vn_count = 0;

	/* get v,vt and vn data from obj file */
	while(1){
		if( fscanf(fp, "%s", tmpc) == EOF ){
			break;
		}

		if(strcmp(tmpc, "v") == 0){
			fscanf(fp, "%f", &model.v[v_count]);
			v_count++;
			fscanf(fp, "%f", &model.v[v_count]);
			v_count++;
			fscanf(fp, "%f", &model.v[v_count]);
			v_count++;
		}

		if(strcmp(tmpc, "vt") == 0){
			fscanf(fp, "%f", &model.vt[vt_count]);
			vt_count++;
			fscanf(fp, "%f", &model.vt[vt_count]);
			vt_count++;
			fscanf(fp, "%f", &model.vt[vt_count]);
			vt_count++;
		}

		if(strcmp(tmpc, "vn") == 0){
			fscanf(fp, "%f", &model.vn[vn_count]);
			vn_count++;
			fscanf(fp, "%f", &model.vn[vn_count]);
			vn_count++;
			fscanf(fp, "%f", &model.vn[vn_count]);
			vn_count++;
		}
	}

	glGenBuffers(1, &model.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.v), model.v, GL_STATIC_DRAW);

	glGenBuffers(1, &model_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.vn), model.vn, GL_STATIC_DRAW);

	model.v_size = sizeof(GLfloat) * v_count;
	model.vt_size = sizeof(GLfloat) * vt_count;
	model.vn_size = sizeof(GLfloat) * vn_count;

	model.v_count = v_count;
	model.vt_count = vt_count;
	model.vn_count = vn_count;
	model.size = v_count / 3;

	return model;
}

void
h_ModelRender(h_Model *model){
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glEnableVertexAttribArray(model->shaderID);
	glVertexAttribPointer( model->shaderID, 3,
			       GL_FLOAT, GL_FALSE,
			       0, (void *)0 );
	
	glDrawArrays(model->render_mode, 0, model->size);
}

void
h_ModelUpdate(h_Model *model){
	glm_perspective(
		model->fAngle,
		model->fAspectRatio,
		model->fNear,
		model->fFar,
		model->m4Projection );

	glm_lookat(
		model->v3Camera,
		model->v3Origin,
		model->v3Up,
		model->m4View );

	glm_mat4_mulN( (mat4 *[]){&model->m4Projection, &model->m4View, &model->m4Model}, 3 , model->m4MVP );
}

void
h_ModelDestroy(h_Model *model){
	glDeleteBuffers(1, &model->VBO);
	model->Render = NULL;
	model->Update = NULL;
	model->Destroy = NULL;
}
