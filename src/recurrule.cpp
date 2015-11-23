#include "recurrule.h"
#include <wx/tokenzr.h>

RecurRule::RecurRule(wxString sRule) :
    m_nFreq(RECUR_NONE),
    m_nCount(0),
    m_nDays(0),
    m_nDaysOfMonth(0),
    m_sRule(sRule)

{
    Parse();
}

RecurRule::~RecurRule()
{
}

void RecurRule::Parse()
{
	//Recur rule
	wxStringTokenizer stParams(m_sRule, ";");
	while(stParams.HasMoreTokens())
	{
		wxString sToken = stParams.GetNextToken();
		wxString sParam = sToken.BeforeFirst('=');
		wxString sValue = sToken.AfterFirst('=');
		if(sParam == "FREQ")
		{
			if(sValue == "DAILY")
				m_nFreq = RECUR_DAILY;
			if(sValue == "WEEKLY")
				m_nFreq = RECUR_WEEKLY;
			if(sValue == "MONTHLY")
				m_nFreq = RECUR_MONTHLY;
			if(sValue == "YEARLY")
				m_nFreq = RECUR_YEARLY;
		}
		else if(sParam == "COUNT")
		{
		    long lValue;
			sValue.ToLong(&lValue);
			m_nCount = (unsigned int)lValue;
		}
		else if(sParam == "BYDAY")
		{
			if(sValue.Contains("SU"))
				m_nDays |= RECUR_SUN;
			if(sValue.Contains("MO"))
				m_nDays |= RECUR_MON;
			if(sValue.Contains("TU"))
				m_nDays |= RECUR_TUE;
			if(sValue.Contains("WE"))
				m_nDays |= RECUR_WED;
			if(sValue.Contains("TH"))
				m_nDays |= RECUR_THU;
			if(sValue.Contains("FR"))
				m_nDays |= RECUR_FRI;
			if(sValue.Contains("SA"))
				m_nDays |= RECUR_SAT;
		}
		else if(sParam == "BYMONTHDAY")
		{
		    long lValue;
		    sValue.ToLong(&lValue);
		    m_nDaysOfMonth |= (1 << lValue);
		}
		else if(sParam == "BYMONTH")
		{
		    long lValue;
		    sValue.ToLong(&lValue);
		    m_nMonths |= (1 << lValue);
		}
	}
}

wxString RecurRule::GetFreqAsString()
{
    switch(m_nFreq)
    {
        case RECUR_SECONDLY:
            return "Secondly";
        case RECUR_MINUTELY:
            return "Minutely";
        case RECUR_HOURLY:
            return "Hourly";
        case RECUR_DAILY:
            return "Daily";
        case RECUR_WEEKLY:
            return "Weekly";
        case RECUR_MONTHLY:
            return "Monthly";
        case RECUR_YEARLY:
            return "Yearly";
        case RECUR_NONE:
        default:
            return "None";
    }
}

wxString RecurRule::GetDaysAsString()
{
    wxString sDays;
    if(OccursOnDays(RECUR_SUN))
        sDays = "Sun";
    if(OccursOnDays(RECUR_MON))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Mon";
    }
    if(OccursOnDays(RECUR_TUE))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Tue";
    }
    if(OccursOnDays(RECUR_WED))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Wed";
    }
    if(OccursOnDays(RECUR_THU))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Thu";
    }
    if(OccursOnDays(RECUR_FRI))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Fri";
    }
    if(OccursOnDays(RECUR_SAT))
    {
        if(!sDays.IsEmpty())
            sDays += ",";
        sDays += "Sat";
    }
    return sDays;
}

bool RecurRule::IsValid()
{
    bool bValid = true;
    if(m_nFreq == RECUR_NONE)
        bValid = false;
    //!@todo Check whole rule
    return bValid;
}

