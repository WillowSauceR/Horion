#include "ClickGui.h"


// State
int i;
// Render
float yOffset1;
float xOffset1;
std::vector<IModule*> CmoduleList;

bool isdown;

void ClickGui::getModuleListByCategory(Category category) {
	std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

	for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
		if ((*it)->getCategory() == category)
			CmoduleList.push_back(*it);
	}
}

void ClickGui::renderLabel(const char * text)
{
	static constexpr float textPadding = 1.0f;
	static constexpr float textSize = 1.0f;
	static constexpr float textHeight = textSize * 10.0f;

	Category cat;
	if (i == 0)
		cat = COMBAT;
	else if (i == 1)
		cat = VISUAL;
	else if (i == 2)
		cat = MOVEMENT;
	else if (i == 3)
		cat = BUILD;
	else if (i == 4)
		cat = EXPLOITS;

	getModuleListByCategory(cat);

	float maxLenght = 1;

	for (auto it = CmoduleList.begin(); it != CmoduleList.end(); ++it) {
		std::string label = (*it)->getModuleName();
		maxLenght = max(maxLenght, DrawUtils::getTextWidth(&label, textSize, SMOOTH));
	}

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	mousePos.div(windowSizeReal);
	mousePos.mul(windowSize);

	DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame,
		g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.025f);

	std::string textStr = text;
	float textWidth = DrawUtils::getTextWidth(&textStr);

	vec2_t textPos = vec2_t(
		xOffset1 + textPadding,
		yOffset1 + textPadding
	);
	vec4_t rectPos = vec4_t(
		xOffset1,
		yOffset1,
		xOffset1 + maxLenght + 10.5f,
		yOffset1 + textHeight
	);
	DrawUtils::drawText(textPos, &textStr, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
	DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), 1.0f);
	GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f,false);
	yOffset1 += textHeight + (textPadding * 2);

	// Loop through mods to display Labels
	for (std::vector<IModule*>::iterator it = CmoduleList.begin(); it != CmoduleList.end(); ++it) {

		textStr = (*it)->getModuleName();
		textWidth = DrawUtils::getTextWidth(&textStr);

		textPos = vec2_t(
			xOffset1 + textPadding,
			yOffset1 + textPadding
		);
		rectPos = vec4_t(
			xOffset1,
			yOffset1,
			xOffset1 + maxLenght + 10.5f,
			yOffset1 + textHeight
		);
		DrawUtils::drawText(textPos, &textStr, (*it)->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos)) {

			DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f),1.0f);
			if (isdown) {
				(*it)->toggle();
				isdown = false;
			}
		}
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f,true);

		yOffset1 += textHeight + (textPadding * 2);
	}
	DrawUtils::fillRectangle(vec4_t(xOffset1, 4, rectPos.z,yOffset1), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.4f);
	DrawUtils::flush();
	CmoduleList.clear();
	yOffset1 = 4;
	i++;
	if (i > 4)
		i = 0;
	xOffset1 += 100;
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;
	yOffset1 = 4;
	xOffset1 = 150;
	// Render all categorys
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Build");
	renderLabel("Exploits");
}

void ClickGui::init() {
	i = 0;
	yOffset1 = 4;
	xOffset1 = 100;
}

void ClickGui::onMouseClickUpdate(bool isDown)
{
	isdown = isDown;
}