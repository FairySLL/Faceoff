#include "Faceoff.h"
//REMINDER: Use initializer lists for constructors u goof

Player::Player(SDL_FRect rect, SDL_FRect hpBar, SDL_Color color, SDL_Renderer* renderer, int maxHp, string name):
	rect(rect),
	hpBar(hpBar),
	color(color),
	renderer(renderer),
	currentHp(maxHp),
	maxHp(maxHp),
	name(name)
	
{}

void Player::renderPlayer() const {
	// Draw player (their own color)
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}

void Player::renderHPBar() const {

	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // MAGIC NUMBER ALERT !!!!
	SDL_RenderFillRect(renderer, &hpBar);

	float healthPercent = (float)currentHp / maxHp;
	int healthWidth = (int)(hpBar.w * healthPercent);

	// Draw HP bar (red)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //MAGIC NUMBER ALERT !!!!
	SDL_FRect currentHPBar = hpBar;
	currentHPBar.w = healthWidth;
	SDL_RenderFillRect(renderer, &currentHPBar);

}





Button::Button(float x, float y, float w, float h, SDL_Color colorNormal, SDL_Color colorHover, SDL_Color colorClicked, const string& text, TTF_Font* font, SDL_Renderer* renderer)
: rect{ x, y, w, h },
colorNormal(colorNormal),
colorHover(colorHover),
colorClicked(colorClicked),
text(text),
font(font),
textTexture(nullptr),
renderer(renderer),
isClicked(false),
isHovered(false)
{}

Button::~Button() {
	if (textTexture) {
		SDL_DestroyTexture(textTexture);
		textTexture = nullptr;
	}
}

void Button::setText(const string& newText) {
	text = newText;
	SDL_Color color = { 255,255,255,255 };//getColorNormal(); MAGIC NUMBER ALERT !!!!!

	// Destroy old texture if it exists
	if (textTexture) {
		SDL_DestroyTexture(textTexture);
		textTexture = nullptr;
	}

	// Create surface from text
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
	if (!textSurface) {
		SDL_Log("Failed to render text surface: %s", SDL_GetError());
		return;
	}

	// Create texture from surface
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
	}

	// Clean up surface
	SDL_DestroySurface(textSurface);
}





bool PointInRectF(float px, float py, const SDL_FRect& rect) {
	return (px >= rect.x && px <= rect.x + rect.w && py >= rect.y && py <= rect.y + rect.h);
}

void Button::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_MOUSE_MOTION) {
		isHovered = PointInRectF(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y), rect);
	}

	else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		if (PointInRectF(static_cast<float>(event.button.x), static_cast<float>(event.button.y), rect)) {
			isClicked = true;
			SDL_Delay(100);
			cout << "Button clicked!\n";
		}
	}

	else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
		isClicked = false;
	}
}


void Button::render(SDL_Renderer* renderer) const {
	//Button background
	if (isClicked)
		SDL_SetRenderDrawColor(renderer, colorClicked.r, colorClicked.g, colorClicked.b, colorClicked.a);
	else if (isHovered)
		SDL_SetRenderDrawColor(renderer, colorHover.r, colorHover.g, colorHover.b, colorHover.a);
	else
		SDL_SetRenderDrawColor(renderer, colorNormal.r, colorNormal.g, colorNormal.b, colorNormal.a);

	SDL_RenderFillRect(renderer, &rect);

	
	//Text
	if (textTexture) {
		float textW = 0.0f, textH = 0.0f;
		if (!SDL_GetTextureSize(textTexture, &textW, &textH)) {
			SDL_Log("Failed to get texture size: %s", SDL_GetError());
		}
		else {
			SDL_FRect dstRect;
			dstRect.x = rect.x + (rect.w - textW) * 0.5f;
			dstRect.y = rect.y + (rect.h - textH) * 0.5f;
			dstRect.w = textW;
			dstRect.h = textH;

			SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect);
		}
	}
}

Textbox::Textbox(SDL_FRect rect, SDL_Color colorRect, SDL_Color colorBorder, SDL_Color colorText, const string& text, TTF_Font* font,
	 SDL_Renderer* renderer)
:	rect{rect},
	colorRect(colorRect),
	colorBorder(colorBorder),
	colorText(colorText),
	text(text),
	font(font),
	textTexture(nullptr),
	renderer(renderer)
{}

Textbox::~Textbox(){
	if (textTexture) {
		SDL_DestroyTexture(textTexture);
		textTexture = nullptr;
		}
	};

void Textbox::setBorder()
{
	float outerX = rect.x - paddingX;
	float outerY = rect.y - paddingY;
	float outerW = rect.w + 2.0f * paddingX;
	float outerH = rect.h + 2.0f * paddingY;

	// Top
	borderTop = {
		outerX,
		outerY,
		outerW,
		borderThickness
	};

	// Bottom
	borderBottom = {
		outerX,
		outerY + outerH - borderThickness,
		outerW,
		borderThickness
	};

	// Left
	borderLeft = {
		outerX,
		outerY,
		borderThickness,
		outerH
	};

	// Right
	borderRight = {
		outerX + outerW - borderThickness,
		outerY,
		borderThickness,
		outerH
	};
}


void Textbox::drawTextbox() 
{
	setBorder();

	SDL_SetRenderDrawColor(renderer, colorBorder.r, colorBorder.g, colorBorder.b, colorBorder.a);
	SDL_RenderFillRect(renderer, &borderTop);
	SDL_RenderFillRect(renderer, &borderBottom);
	SDL_RenderFillRect(renderer, &borderLeft);
	SDL_RenderFillRect(renderer, &borderRight);

	SDL_SetRenderDrawColor(renderer, colorRect.r, colorRect.g, colorRect.b, colorRect.a);
	SDL_RenderFillRect(renderer, &rect);

	if (textTexture) {
		float textW = 0.0f, textH = 0.0f;
		if (!SDL_GetTextureSize(textTexture, &textW, &textH)) {
			SDL_Log("Failed to get texture size: %s", SDL_GetError());
		}
		else {
			SDL_FRect dstRect;
			dstRect.x = rect.x + (rect.w - textW) * 0.5f;
			dstRect.y = rect.y + (rect.h - textH) * 0.5f;
			dstRect.w = textW;
			dstRect.h = textH;

			SDL_SetRenderDrawColor(renderer, colorText.r, colorText.g, colorText.b, colorText.a);
			SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect);
		}
	}
}

void Textbox::setTextBox(const string& newText) {

	text = newText;
	SDL_Color color = { 255,255,255,255 };//getColorNormal(); MAGIC NUMBER ALERT !!!!!

	if (textTexture) {
		SDL_DestroyTexture(textTexture);
		textTexture = nullptr;
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
	if (!textSurface) {
		SDL_Log("Failed to render text surface: %s", SDL_GetError());
		return;
	}


	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
	}

	SDL_DestroySurface(textSurface);
}






