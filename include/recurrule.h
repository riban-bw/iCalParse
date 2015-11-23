/***************************************************************
 * Name:      recurrule.h
 * Purpose:   Defines recur rule class
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2015-11-24
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#pragma once
#include <wx/string.h>
#include <stdint.h>

enum EVENT_RECUR_FREQ
{
    RECUR_NONE,
    RECUR_SECONDLY,
    RECUR_MINUTELY,
    RECUR_HOURLY,
    RECUR_DAILY,
    RECUR_WEEKLY,
    RECUR_MONTHLY,
    RECUR_YEARLY
};

enum EVENT_RECUR_DAY
{
    RECUR_SUN = 0x01,
    RECUR_MON = 0x02,
    RECUR_TUE = 0x04,
    RECUR_WED = 0x06,
    RECUR_THU = 0x10,
    RECUR_FRI = 0x20,
    RECUR_SAT = 0x40
};

enum EVENT_RECUR_MONTH
{
    RECUR_JAN = 0x001,
    RECUR_FEB = 0x002,
    RECUR_MAR = 0x003,
    RECUR_APR = 0x004,
    RECUR_MAY = 0x008,
    RECUR_JUN = 0x010,
    RECUR_JUL = 0x020,
    RECUR_AUG = 0x040,
    RECUR_SEP = 0x080,
    RECUR_OCT = 0x100,
    RECUR_NOV = 0x200,
    RECUR_DEC = 0x400
};

class RecurRule
{
    public:
        RecurRule(wxString sRule);
        virtual ~RecurRule();

        //Getters
        /** @brief  Get the raw rule string
        *   @return <i>wxString</i> Raw rule
        */
        wxString GetRule() { return m_sRule; }

        /** @brief  Get the frequency of the rule as a EVENT_RECUR_FREQ
        *   @return <i>EVENT_RECUR_FREQ</i> The recur frequency
        */
        EVENT_RECUR_FREQ GetFreq() { return m_nFreq; }

        /** @brief  Get the frequency of the rule as a string
        *   @return <i>wxString</i> The recur frequency
        */
        wxString GetFreqAsString();

        /** @brief  Get the quantity of recurrences
        *   @return <i>long</i> Quantity of recurrences
        */
        unsigned int GetCount() { return m_nCount; }

        /** @brief  Get the days the event occurs on
        *   @return <i>unsigned int</i> Bitwise flag of days the event occurs from [EVENT_DAY]
        */
        unsigned int GetDays() { return m_nDays; }

        /** @brief  Get the days of month the event occurs on
        *   @return <i>uint32_t</i> Bitwise flag of days of month the event occurs from EVENT_RECUR_MONTH
        */
        uint32_t GetDaysOfMonth() { return m_nDaysOfMonth; }

        /** @brief  Get the days the event occurs on as string
        *   @return <i>wxString</i> Comma delimited list of days, e.g. "Mon, Fri"
        */
        wxString GetDaysAsString();

        /** @brief   Check if recurs on specific day
        *   @param   nDays Day of week (bitwise day flags from [EVENT_DAY])
        *   @return  <i>bool</i> True if event occurs on all specified day
        */
        bool OccursOnDays(unsigned int nDays) { return (m_nDays & nDays) == nDays; }

        /** @brief  Check if recur rule is valid
        *   @return <i>bool</i> True if valid
        */
        bool IsValid();

        void SetRule(wxString val) { m_sRule = val; }

    protected:
    private:
        void Parse(); //Parse the rule
        EVENT_RECUR_FREQ m_nFreq; //Frequency of the rule [EVENT_RECUR_REQ]
        unsigned int m_nCount; //Quantity of recurrences
        unsigned int m_nDays; //bitwise flag of recurrence days (0x01=Sun, 0x02=Tue)
        uint32_t m_nDaysOfMonth; //Bitwise flag of days of month [EVENT_RECUR_MONTH]
        unsigned int m_nMonths; //Bitwise flag of months (0x01=Jan)
        wxString m_sRule;
};
