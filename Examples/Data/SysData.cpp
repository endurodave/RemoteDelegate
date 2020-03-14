#include "SysData.h"

std::ostream& operator<< (std::ostream &out, const SystemModeChanged& data)
{
    out.write(reinterpret_cast<const char*>(&data.PreviousSystemMode), sizeof(SystemMode::Type));
    out.write(reinterpret_cast<const char*>(&data.CurrentSystemMode), sizeof(SystemMode::Type));
    return out;
}
std::istream& operator>> (std::istream &in, const SystemModeChanged& data)
{
    SystemModeChanged& d = const_cast<SystemModeChanged&>(data);
    in.read(reinterpret_cast<char*>(&d.PreviousSystemMode), sizeof(SystemMode::Type));
    in.read(reinterpret_cast<char*>(&d.CurrentSystemMode), sizeof(SystemMode::Type));
    return in;
}

MulticastDelegateSafe1<const SystemModeChanged&> SysData::SystemModeChangedDelegate;

//----------------------------------------------------------------------------
// GetInstance
//----------------------------------------------------------------------------
SysData& SysData::GetInstance()
{
	static SysData instance;
	return instance;
}

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
SysData::SysData() :
	m_systemMode(SystemMode::STARTING)
{
	LockGuard::Create(&m_lock);
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
SysData::~SysData()
{
	LockGuard::Destroy(&m_lock);
}

//----------------------------------------------------------------------------
// SetSystemMode
//----------------------------------------------------------------------------
void SysData::SetSystemMode(SystemMode::Type systemMode)
{
	LockGuard lockGuard(&m_lock);

	// Create the callback data
	SystemModeChanged callbackData;
	callbackData.PreviousSystemMode = m_systemMode;
	callbackData.CurrentSystemMode = systemMode;

	// Update the system mode
	m_systemMode = systemMode;

	// Callback all registered subscribers
	if (SystemModeChangedDelegate)
		SystemModeChangedDelegate(callbackData);
}

