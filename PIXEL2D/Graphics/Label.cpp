#include "Label.h"
#include "..//Core/Transform.h"

namespace PIXL{ namespace graphics{

	Label::Label(const string& fontPath, const string& text, const math::Vector4f& color /*= math::Vector4f::One()*/, unsigned int size)
		: Drawable(math::Vector2f::One(), color)
	{
		m_font = new Font(fontPath, size);
		m_text = text;
	}

	Label::Label(Font* font, const string& text, const math::Vector4f& color /*= math::Vector4f::One()*/)
	{
		m_font = font;
		m_text = text;
	}

	void Label::Draw(Renderer* renderer)
	{
		renderer->SubmitLabel(m_text, entity->GetTransform().GetPosition(), *m_font, m_color);
	}

	void Label::SetText(const string& text)
	{
		m_text = text;
	}

	const string& Label::GetText()
	{
		return m_text;
	}

	const void Label::SetFontSize(unsigned int size)
	{
		m_font->SetFontSize(size);
	}

}
}