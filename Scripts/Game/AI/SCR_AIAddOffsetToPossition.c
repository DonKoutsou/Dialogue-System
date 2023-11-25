class SCR_AIAddOffsetToPosition: AITaskScripted
{
	[Attribute("1.5", UIWidgets.EditBox, "Vertical offset to add to position" )]
	protected float m_fVerticalOffset;
	protected static const string POSITION_CORRECTION_PORT = "PositionOut";
	protected static const string POSITION_PORT = "PositionIn";
	//-----------------------------------------------------------------------------------------------
	override bool VisibleInPalette() {return true;}
	
	//-----------------------------------------------------------------------------------------------
	protected static ref TStringArray s_aVarsIn = {
		POSITION_PORT
	};
	override array<string> GetVariablesIn()
	{
		return s_aVarsIn;
	}
	
	protected static ref TStringArray s_aVarsOut = {
		POSITION_CORRECTION_PORT
	};
	override array<string> GetVariablesOut()
	{
		return s_aVarsOut;
	}
	
	//-----------------------------------------------------------------------------------------------
	override ENodeResult EOnTaskSimulate(AIAgent owner, float dt)
	{
		
		vector lookPosition;
		
		GetVariableIn(POSITION_PORT, lookPosition);
		lookPosition[1] = lookPosition[1] + m_fVerticalOffset;
		
		SetVariableOut(POSITION_CORRECTION_PORT,lookPosition);
		return ENodeResult.SUCCESS;
    }
};