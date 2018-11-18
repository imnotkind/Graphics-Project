#include "CDrawing.h"

CDrawing::CDrawing(const SDrawingInfo& s)
{
	V_PSM = CShaderManager::getInstance();
	V_Array = V_PSM->M_GetPolygon(s.PolygonName);
	V_Color = s.Global_Color;
	V_Program = s.Program;
	V_DrawMode = s.DrawMode;
	V_LineColor = s.Line_Color;
}

void CDrawing::M_Draw(const glm::mat4& mat, T4Double color)
{
	if (V_Array.aindex == -1) return; //dummy draw

	glBindVertexArray(V_Array.aindex);
	GLuint p = glGetUniformLocation(V_PSM->M_GetProgram() , "trans");
	GLuint q = glGetUniformLocation(V_PSM->M_GetProgram(), "vicolor");
	glUniformMatrix4fv(p, 1, GL_FALSE, &mat[0][0]);
	float col[4];
	for (int i = 0; i< 4; i++) col[i] = V_Color[i] * color[i];
	glUniform4fv(q, 1, col);


	if (V_DrawMode == 2) glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num); 
	else if (V_DrawMode == 1) glDrawArrays(GL_LINE_STRIP, 0, V_Array.num); //wireframe
	else if (V_DrawMode == 3) // wireframe + color
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num);
		for (int i = 0; i < 4; i++) col[i] = V_LineColor[i];// *color[i];
		glUniform4fv(q, 1, col);

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
		glUniform4fv(q, 1, col);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // no fill
		glDrawArrays(GL_TRIANGLES, 0, V_Array.num);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // back to default
	}
	else if (V_DrawMode == 6)
	{
		glDrawArrays(GL_LINES, 0, V_Array.num);
	}
}

