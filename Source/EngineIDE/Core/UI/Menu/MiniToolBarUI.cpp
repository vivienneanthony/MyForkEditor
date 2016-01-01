#include "EngineIDEStd.h"

#include "MiniToolBarUI.h"

#include "Urho3D/UI/ToolTip.h"

MiniToolBarUI::MiniToolBarUI(Context* context) : BorderImage(context)
{
	bringToFront_ = true;
	clipChildren_ = true;
	SetEnabled(true);
	SetLayout(LM_VERTICAL, 4, IntRect(4, 4, 4, 4));
	SetAlignment(HA_LEFT, VA_TOP);
}

MiniToolBarUI::~MiniToolBarUI()
{

}

void MiniToolBarUI::RegisterObject(Context* context)
{
	context->RegisterFactory<MiniToolBarUI>();
	URHO3D_COPY_BASE_ATTRIBUTES(BorderImage);
	URHO3D_UPDATE_ATTRIBUTE_DEFAULT_VALUE("Is Enabled", true);
}

MiniToolBarUI* MiniToolBarUI::Create(UIElement* parent, const String& idname, XMLFile* iconStyle,  int width, int height , XMLFile* defaultstyle )
{
    MiniToolBarUI* menubar = parent->CreateChild<MiniToolBarUI>(idname);
	
	if (defaultstyle)
	{
		menubar->SetDefaultStyle(defaultstyle);
	}

    menubar->SetStyleAuto();
	
	if (height > 0)
	{
		menubar->SetFixedHeight(height);
	}
	else
	{
		menubar->SetFixedHeight(parent->GetRoot()->GetHeight() - parent->GetMinHeight());
	}
        
	menubar->SetFixedWidth(width);
	menubar->m_pIconStyle = iconStyle;

    return menubar;
}

UIElement* MiniToolBarUI::CreateSmallToolBarButton(const String& title, const String& toolTipTitle /*= String::EMPTY*/)
{
    Button* button = new Button(context_);
    button->SetName(title);
    button->SetDefaultStyle(GetDefaultStyle());
    button->SetStyle("ToolBarButton");
    button->SetFixedSize(20, 20);
    
	CreateSmallToolBarIcon(button);

	if (toolTipTitle.Empty())
	{
		CreateToolTip(button, title, IntVector2(button->GetWidth() + 10, button->GetHeight() - 10));
	}
	else
	{
		CreateToolTip(button, toolTipTitle, IntVector2(button->GetWidth() + 10, button->GetHeight() - 10));
	}

    AddChild(button);
    return button;
}

UIElement* MiniToolBarUI::CreateSmallToolBarSpacer(unsigned int width)
{
    UIElement* spacer = CreateChild<UIElement>("Spacer");
    spacer->SetFixedHeight(width);
    return spacer;
}

void MiniToolBarUI::CreateSmallToolBarIcon(UIElement* element)
{
    BorderImage* icon = new BorderImage(context_);
    icon->SetName("Icon");
	icon->SetDefaultStyle(m_pIconStyle);
    icon->SetStyle(element->GetName());
    icon->SetFixedSize(14, 14);
    element->AddChild(icon);
}

UIElement* MiniToolBarUI::CreateToolTip(UIElement* parent, const String& title, const IntVector2& offset)
{
    ToolTip* toolTip = parent->CreateChild<ToolTip>("ToolTip");
    toolTip->SetPosition(offset);

    BorderImage* textHolder = toolTip->CreateChild<BorderImage>("BorderImage");
    textHolder->SetStyle("ToolTipBorderImage");

    Text* toolTipText = textHolder->CreateChild<Text>("Text");
    toolTipText->SetStyle("ToolTipText");
    toolTipText->SetText(title);

    return toolTip;
}









