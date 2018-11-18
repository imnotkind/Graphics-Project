#include "CDrawing.h"

CDrawing::CDrawing(const SDrawingInfo& s)
{
	V_PSM = CShaderManager::getInstance();
	V_Array = V_PSM->M_GetPolygon(s.PolygonName);
	V_Program = s.Program;
	V_DrawMode = s.DrawMode;
}

void CDrawing::M_Draw(const glm::mat4& mat, T4Double color)
{
	if (V_Array.aindex == -1) return; //dummy draw
	auto x = CShaderManager::getInstance();
	x->M_UseProgram(V_Program);


	glBindVertexArray(V_Array.aindex);
	GLuint p = glGetUniformLocation(V_PSM->M_GetProgram() , "trans");
	glUniformMatrix4fv(p, 1, GL_FALSE, &mat[0][0]);

	if (V_DrawMode == 2) glDrawArrays(GL_TRIANGLE_STRIP, 0, V_Array.num); 
	else if (V_DrawMode == 1) glDrawArrays(GL_LINE_STRIP, 0, V_Array.num); 
	else if (V_DrawMode == 3)  glDrawArrays(GL_TRIANGLES, 0, V_Array.num); 
	else if (V_DrawMode == 0)glDrawArrays(GL_POINTS, 0, V_Array.num); 

}

