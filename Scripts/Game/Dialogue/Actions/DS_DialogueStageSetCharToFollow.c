[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageSetCharacterToFollowAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		super.Perform(Character, Player);
		AIControlComponent comp = AIControlComponent.Cast(Character.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		SCR_AIFollowBehavior action = new SCR_AIFollowBehavior(utility, null, Player);
		utility.AddAction(action);
		//if player throw popup
		if (GetGame().GetPlayerController().GetControlledEntity() == Character)
		{
			SCR_HintManagerComponent.GetInstance().ShowCustom(string.Format("%1 started to follow you", DS_DialogueComponent.GetCharacterName(Character)));
		}	
		
	};
};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageStopFollowingAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		super.Perform(Character, Player);
		AIControlComponent comp = AIControlComponent.Cast(Character.FindComponent(AIControlComponent));
		AIAgent agent = comp.GetAIAgent();
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		SCR_AIFollowBehavior act = SCR_AIFollowBehavior.Cast(utility.FindActionOfType(SCR_AIFollowBehavior));
		if (act)
			act.SetActiveFollowing(false);
		//if player throw popup
		SCR_HintManagerComponent.GetInstance().ShowCustom(string.Format("%1 has stopped following you.", DS_DialogueComponent.GetCharacterName(Character)));
	};
};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageAssignBaseWPAction : DS_BaseDialogueStageAction
{
	[Attribute(defvalue: "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", UIWidgets.ResourceNamePicker, desc: "Default waypoint prefab", "et", category: "Defender Spawner")]
	ResourceName m_sDefaultWaypointPrefab;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		vector spawnpos;
		Resource wpRes;
		SCR_AIWaypoint wp;
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		SCR_GameModeCampaign gamemod = SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
		SCR_CampaignMilitaryBaseManager Baseman = gamemod.GetBaseManager();
		vector pos = Character.GetOrigin();
		SCR_CampaignMilitaryBaseComponent base = Baseman.FindClosestBase(pos);
		if (!base)
			return;
		base.GetOwner().GetTransform(params.Transform);
		
		wpRes = Resource.Load(m_sDefaultWaypointPrefab);
		if (!wpRes.IsValid())
			return;
		wp = SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(wpRes, null, params));
		if (!wp)
			return;
		ChimeraCharacter char = ChimeraCharacter.Cast(Character);
		
		SCR_AIGroup group = SCR_AIGroup.Cast(char.GetCharacterController().GetAIControlComponent().GetAIAgent().GetParentGroup());
		
		group.AddWaypoint(wp);
	};
};


