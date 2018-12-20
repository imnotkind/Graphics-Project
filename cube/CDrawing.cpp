#include "CDrawing.h"

CDrawing::CDrawing(const SDrawingInfo& s)
{
	V_PSM = CShaderManager::getInstance();
	V_Array = V_PSM->M_GetPolygon(s.PolygonName);
	V_Color = s.Global_Color;
	V_Program = s.Program;
	V_DrawMode = s.DrawMode;
	V_LineColor = s.Line_Color;
	V_Texture = s.texture;
	V_Light = s.light;
}

void CDrawing::M_Draw(const SRenderInfo& r)
{
	if (V_Array.aindex == -1) return; //dummy draw

	V_PSM->M_UseProgram(V_Program);

	glBindVertexArray(V_Array.aindex);

	GLuint p;
	p = glGetUniformLocation(V_PSM->M_GetProgram(), "projection");
	glUniformMatrix4fv(p, 1, GL_FALSE, &r.projection[0][0]);
	p = glGetUniformLocation(V_PSM->M_GetProgram(), "model");
	glUniformMatrix4fv(p, 1, GL_FALSE, &r.model[0][0]);
	p = glGetUniformLocation(V_PSM->M_GetProgram(), "view");
	glUniformMatrix4fv(p, 1, GL_FALSE, &r.view[0][0]);

	p = glGetUniformLocation(V_PSM->M_GetProgram(), "vicolor");
	float col[4];
	for (int i = 0; i< 4; i++) col[i] = V_Color[i] * r.color[i];
	glUniform4fv(p, 1, col);

	p = glGetUniformLocation(V_PSM->M_GetProgram(), "normaltrans");
	glUniformMatrix4fv(p, 1, GL_FALSE, &r.normtrans[0][0]);

	if (V_Light && !r.keeplight)
	{


		p = glGetUniformLocation(V_PSM->M_GetProgram(), "ambient");
		glUniform4fv(p, 1, &r.amb[0]);

		for (int i = 0; i < r.lights.size(); i++)
		{
			auto l = r.lights[i];
			char buf[30];
			sprintf(buf, "light[%d].%s", i, "diffuse"); //WTF sprintf is too slow
			p = glGetUniformLocation(V_PSM->M_GetProgram(), buf);
			glUniform4fv(p, 1, &l.dif[0]);

			sprintf(buf, "light[%d].%s", i, "specular");
			p = glGetUniformLocation(V_PSM->M_GetProgram(), buf);
			glUniform4fv(p, 1, &l.spc[0]);

			sprintf(buf, "light[%d].%s", i, "pos");
			p = glGetUniformLocation(V_PSM->M_GetProgram(), buf);
			glUniform4fv(p, 1, &l.pos[0]);
		}

	}
	if (V_Texture != -1)
	{
		p = glGetUniformLocation(V_PSM->M_GetProgram(), "texsam");
		glUniform1i(p, 0);
	}


	if (V_DrawMode == 2) glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num);
	else if (V_DrawMode == 1) glDrawArrays(GL_LINE_STRIP, 0, V_Array.num); //wireframe
	else if (V_DrawMode == 3) // wireframe + color
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num);
		for (int i = 0; i < 4; i++) col[i] = V_LineColor[i];// *color[i];
		p = glGetUniformLocation(V_PSM->M_GetProgram(), "vicolor");
		glUniform4fv(p, 1, col);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // no fill
		glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // back to default
	}
	else if (V_DrawMode == 0)glDrawArrays(GL_POINTS, 0, V_Array.num);
	else if (V_DrawMode == 4)glDrawArrays(GL_TRIANGLES, 0, V_Array.num);
	else if (V_DrawMode == 5)
	{
		glDrawArrays(GL_TRIANGLES, 0, V_Array.num);
		for (int i = 0; i < 4; i++) col[i] = V_LineColor[i];// *color[i];
		p = glGetUniformLocation(V_PSM->M_GetProgram(), "vicolor");
		glUniform4fv(p, 1, col);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // no fill
		glDrawArrays(GL_TRIANGLES, 0, V_Array.num);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // back to default
	}
	else if (V_DrawMode == 6)
	{
		glDrawArrays(GL_LINES, 0, V_Array.num);
	}
}
