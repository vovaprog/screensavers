#pragma once

class MovingValue
{
private:
	float normalDValue;
	float dIncValue;
	float downBreakBound;
	float upBreakBound;	
	
	float d;
	bool stateNormal;
	
	float dTarget;
	float dInc;
	
	float controlledValue;
	
public:
	MovingValue(){};
	
	MovingValue(
		float normalDValue,
		float dIncValue,
		float downBreakBound,
		float upBreakBound,		
		float initialDValue,
		float initialControlledValue):
			normalDValue(normalDValue),
			dIncValue(dIncValue),
			downBreakBound(downBreakBound),
			upBreakBound(upBreakBound),			
			d(initialDValue),
			stateNormal(true),
			controlledValue(initialControlledValue)
	{		
	}
	
private:		

	inline void startUpBreak()
	{	
		dInc=-dIncValue;
		dTarget=-normalDValue;
		
		stateNormal=false;
	}

	inline void startDownBreak()
	{
		dInc=dIncValue;
		dTarget=normalDValue;

		stateNormal=false;
	}

	inline bool dReady()
	{
		if((dTarget>0 && d>dTarget) ||
			(dTarget<0 && d<dTarget))	
		{
			return true;
		}
		else
		{
			return false;
		}
	}

public:
	float step()
	{
		if(stateNormal)
		{
			if(d>0 && controlledValue>upBreakBound)
			{
				startUpBreak();
			}
			else if(d<0 && controlledValue<downBreakBound)
			{
				startDownBreak();
			}
		}
		else
		{
			d+=dInc;
			
			if(dReady())
			{
				stateNormal=true;
			}
		}
		
		controlledValue+=d;
		
		return controlledValue;
	}
};

