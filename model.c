/* I do not plan to add support for any other model format, Wavefront's format
 * is good enough for me and it is easy to work with in my opinion.
 *
 * NOTE: By default, minimum requirement for model to parse is to have
 * proper v,vt,vn and f entries inside .obj, nothing else.
 *
 * If there are additional supported elements present parser will assign
 * them accordingly.
 *
 * Supported: v,vt,vn,f parsing.
 */

#include "model.h"

h_Model
h_ModelInit( char *path ){

	/* After every model loading this variable increments, thus
	 * ensuring original shader ID for every model. This also
	 * means that you can use unique shader for every model you load.
	 * `TO BE IMPLEMENTED`
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
		fclose(fp);
	}

	char tmpc[100];

	int v_count = 0;
	int vt_count = 0;
	int vn_count = 0;
	int f_v_count = 0;
	int f_vt_count = 0;
	int f_vn_count = 0;

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

		if(strcmp(tmpc, "f") == 0) {
			fscanf(fp, "%hu/%hu/%hu",
			       &model.f_v[f_v_count],
			       &model.f_vt[f_vt_count],
			       &model.f_vn[f_vn_count]);
			f_v_count++; f_vt_count++; f_vn_count++;

			fscanf(fp, "%hu/%hu/%hu",
			       &model.f_v[f_v_count],
			       &model.f_vt[f_vt_count],
			       &model.f_vn[f_vn_count]);
			f_v_count++; f_vt_count++; f_vn_count++;

			fscanf(fp, "%hu/%hu/%hu",
			       &model.f_v[f_v_count],
			       &model.f_vt[f_vt_count],
			       &model.f_vn[f_vn_count]);
			f_v_count++; f_vt_count++; f_vn_count++;
		}
	}

	fclose(fp);

	glGenBuffers(1, &model.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.v), model.v, GL_STATIC_DRAW);

	glGenBuffers(1, &model.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.f_v), model.f_v, GL_STATIC_DRAW);

	model.v_size = sizeof(GLfloat) * v_count;

	model.vt_size = sizeof(GLfloat) * vt_count;
	model.vn_size = sizeof(GLfloat) * vn_count;

	model.f_v_size = sizeof(GLuint) * f_v_count;
	model.f_vt_size = sizeof(GLuint) * f_vt_count;
	model.f_vn_size = sizeof(GLuint) * f_vn_count;

	model.v_count = v_count;
	model.vt_count = vt_count;
	model.vn_count = vn_count;

	model.f_v_count = f_v_count;
	model.f_vt_count = f_vt_count;
	model.f_vn_count = f_vn_count;
	
	model.size = v_count / 3;
	
	return model;
}

void
h_ModelRender(h_Model *model){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	
	glVertexAttribPointer( model->shaderID, 3,
			       GL_FLOAT, GL_FALSE,
			       0, (void *)0 );
	glEnableVertexAttribArray(model->shaderID);
	
	glDrawElements(model->render_mode, model->f_v_count, GL_UNSIGNED_SHORT, NULL);
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
	glDeleteBuffers(1, &model->EBO);
	model->Render = NULL;
	model->Update = NULL;
	model->Destroy = NULL;
}
