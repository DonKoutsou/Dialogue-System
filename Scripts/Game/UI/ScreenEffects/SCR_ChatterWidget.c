class SCR_ChatterWidget : SCR_InfoDisplayExtended
{
	const ResourceName INSPECT_CASUALTY_LAYOUT = "{8998563B2B97D6A8}UI/layouts/Damage/DialogueMenuBubble.layout";
	
	protected SCR_CharacterDamageManagerComponent m_CharDamageManager;
	protected array <IEntity> m_aTargets;
	protected map <IEntity, ref Widget> m_mChatterWidgetMap;
	protected map <IEntity, float> m_mChatterWidgetTimerMap;
	
	protected Widget m_wIDInspectWidget;

	IEntity m_Target;
	string m_sText;
	protected const float UPDATE_FREQ = 0.5;
	protected const string TARGET_BONE = "Spine4";
	protected float m_fTimeTillUpdate;
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
		for (int i = 0; i < m_mChatterWidgetTimerMap.Count(); i++)
		{
			float timer = m_mChatterWidgetTimerMap.GetElement(i);
			timer -= timeSlice;
			if (timer < 0)
			{
				m_mChatterWidgetTimerMap.RemoveElement(i);
				Widget element = m_mChatterWidgetMap.GetElement(i);
				element.SetVisible(false);
				
			}
		}
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

		SCR_DialogueBubble IDInfoUI = SCR_DialogueBubble.Cast(m_wIDInspectWidget.FindHandler(SCR_DialogueBubble));
		if (IDInfoUI)
		{
			IDInfoUI.SetText(m_sText);
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
		if (posX < 0 || posX > winX || posY > winY || posY < 0)
		{
			DisableWidget();
			return;
		}
		
		FrameSlot.SetPos(m_wIDInspectWidget.GetChildren(), pos[0], pos[1]);
				
		float dist = vector.Distance(GetGame().GetPlayerController().GetControlledEntity().GetOrigin(), WPPos);
		if (dist >= 4)
		{
			DisableWidget();
			return;
		}
		
		float opacity = 1;
		float distanceOpacityReduction = 1;
		
		if (dist <= 3)
			distanceOpacityReduction = 0;
		else
			distanceOpacityReduction = Math.InverseLerp(3, 4, dist);
				
		m_wIDInspectWidget.SetOpacity(opacity - distanceOpacityReduction);
	}

	//------------------------------------------------------------------------------------------------
	override protected void DisplayOnSuspended()
	{
		DisableWidget();
	}

	//------------------------------------------------------------------------------------------------
	void AddTarget(IEntity target)
	{
		Widget wid;
		m_mChatterWidgetMap.Insert(target, wid);
		m_mChatterWidgetTimerMap.Insert(target, 10);
		m_Target = target;
	}
	void SetText(string text)
	{
		m_sText = text;
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
