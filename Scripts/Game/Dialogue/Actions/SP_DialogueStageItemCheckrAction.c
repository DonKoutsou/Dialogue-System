[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageItemCheckAction : DS_BaseDialogueStageAction
{
	[Attribute("Item needed to be delivered", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_WantedItem;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
};
