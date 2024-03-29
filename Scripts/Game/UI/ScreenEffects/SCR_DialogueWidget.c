class SCR_DialogueWidget : SCR_InfoDisplayExtended
{
	[Attribute("10")]
	int m_iMaxDistance;
	
	const ResourceName INSPECT_CASUALTY_LAYOUT = "{8998563B2B97D6A8}UI/layouts/Damage/DialogueMenuBubble.layout";
	
	protected SCR_CharacterDamageManagerComponent m_CharDamageManager;
	protected Widget m_wIDInspectWidget;

	IEntity m_Target;
	string m_sText;
	int m_iStringFrame;
	protected const float UPDATE_FREQ = 0.001;
	protected const string TARGET_BONE = "Spine4";
	protected float m_fTimeTillUpdate;
	protected float m_fLifetime;
	protected bool m_bShouldBeVisible;

	//------------------------------------------------------------------------------------------------	
	override void DisplayStartDraw(IEntity owner)
	{
		m_wIDInspectWidget = GetRootWidget();
		DisableWidget();
	}
	
	//------------------------------------------------------------------------------------------------
	override event void DisplayUpdate(IEntity owner, float timeSlice)
	{
		if (m_fTimeTillUpdate > 0)
		{
			m_fTimeTillUpdate -= timeSlice;
		}
		else
		{
			m_fTimeTillUpdate = UPDATE_FREQ;
			UpdateTarget();
		}
		
		UpdateWidget();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Start showing the widget
	bool ShowInspectCasualtyWidget(IEntity targetCharacter)
	{
		if (!m_wIDInspectWidget)
			return false;
		
		ChimeraCharacter char = ChimeraCharacter.Cast(targetCharacter);
		if (!char)
			return false;
		
		CharacterControllerComponent targetController = char.GetCharacterController();
		if (!targetController)
			return false;

		UpdateTarget();
		EnableWidget();	
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! Check if target is still alive and update widget if so
	void UpdateTarget()
	{
		if (!m_Target || !m_wIDInspectWidget)
			return;
		
		ChimeraCharacter char = ChimeraCharacter.Cast(m_Target);
		if (!char)
			return;
		
		SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (controller.GetLifeState() == ECharacterLifeState.DEAD)
		{
			DisableWidget();
			return;
		}

		UpdateWidgetData();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Gather and update data of target character into widget
	void UpdateWidgetData()
	{
		if (!m_Target || !m_wIDInspectWidget)
			return;
		
		if (m_iStringFrame >= m_sText.Length())
			return;
		SCR_DialogueBubble IDInfoUI = SCR_DialogueBubble.Cast(m_wIDInspectWidget.FindHandler(SCR_DialogueBubble));
		if (IDInfoUI)
		{
			m_iStringFrame += 1;
			
			IDInfoUI.SetText(m_sText.Substring(0, m_iStringFrame));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Update widget position and opacity
	void UpdateWidget()
	{
		if (!m_Target || !m_wIDInspectWidget || !m_bIsEnabled)
			return;

		vector boneVector[4];
		m_Target.GetAnimation().GetBoneMatrix(m_Target.GetAnimation().GetBoneIndex(TARGET_BONE), boneVector);
		
		vector WPPos = boneVector[3] + m_Target.GetOrigin();
		vector pos = GetGame().GetWorkspace().ProjWorldToScreen(WPPos, GetGame().GetWorld());
		// Handle off-screen coords
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		int winX = workspace.GetWidth();
		int winY = workspace.GetHeight();
		int posX = workspace.DPIScale(pos[0]);
		int posY = workspace.DPIScale(pos[1]);

		// If widget off screen, remove widget
		/*if (posX < 0 || posX > winX || posY > winY || posY < 0)
		{
			DisableWidget();
			return;
		}*/
		if (!m_fLifetime)
		{
			m_fLifetime = m_sText.Length() * 4;
		}
		m_fLifetime = m_fLifetime - 1;
		if (m_fLifetime <= 0)
		{
			DisableWidget();
			return;
		}
		
		FrameSlot.SetPos(m_wIDInspectWidget.GetChildren(), pos[0], pos[1]);
				
		float dist = vector.Distance(GetGame().GetPlayerController().GetControlledEntity().GetOrigin(), WPPos);
		if (dist >= m_iMaxDistance)
		{
			DisableWidget();
			return;
		}
		
		float opacity = 1;
		float distanceOpacityReduction = 1;
		
		if (dist <= m_iMaxDistance * 0.75)
			distanceOpacityReduction = 0;
		else
			distanceOpacityReduction = Math.InverseLerp(m_iMaxDistance * 0.75, m_iMaxDistance, dist);
				
		m_wIDInspectWidget.SetOpacity(opacity - distanceOpacityReduction);
	}

	//------------------------------------------------------------------------------------------------
	override protected void DisplayOnSuspended()
	{
		DisableWidget();
	}

	//------------------------------------------------------------------------------------------------
	void SetTarget(IEntity target)
	{
		m_Target = target;
	}
	void SetText(string text)
	{
		m_sText = SCR_StringHelper.Translate(text);
		m_iStringFrame = 0;
	}
	//------------------------------------------------------------------------------------------------			
	bool IsActive()
	{
		return m_Target && m_wIDInspectWidget && m_wIDInspectWidget.GetOpacity() != 0;
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableWidget()
	{
		if (m_wIDInspectWidget)
			m_wIDInspectWidget.SetVisible(false);
		
		m_fLifetime = 0;
		m_Target = null;
		SetEnabled(false);		
		m_bShouldBeVisible = false;
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableWidget()
	{
		if (m_wIDInspectWidget)
			m_wIDInspectWidget.SetVisible(true);
		
		SetEnabled(true);		
		m_bShouldBeVisible = true;			
	}
	
	//------------------------------------------------------------------------------------------------
	override void DisplayOnResumed()
	{
		if (!m_bShouldBeVisible && m_wIDInspectWidget)
			m_wIDInspectWidget.SetVisible(false);			
	}
};
