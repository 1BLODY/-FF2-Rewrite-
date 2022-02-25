#include <sdktools_sound>

#if defined _ff2r_included
	#endinput
#endif
#define _ff2r_included

/**
 * Emits a boss's random sound to a list of clients.
 * 
 * @param clients       Array of client indexes
 * @param numClients    Number of clients in the array
 * @param key			Boss's sound key
 * @param boss			Boss's client index
 * @param required		Required value for a sound
 * @param entity		Entity to emit from
 * @param channel		Channel to emit with
 * @param level			Sound level
 * @param flags			Sound flags
 * @param volume		Sound volume
 * @param pitch			Sound pitch
 * @param speakerentity	Unknown
 * @param origin		Sound origin
 * @param dir			Sound direction
 * @param updatePos		Unknown (updates positions?)
 * @param soundtime		Alternate time to play sound for
 * 
 * @error				Invalid client index
 * 
 * @return				True if the sound was found, false otherwise
 */
native bool FF2R_EmitBossSound(const int[] clients, int numClients, const char[] key, int boss, const char[] required="", int entity=SOUND_FROM_PLAYER, int channel=SNDCHAN_AUTO, int level=SNDLEVEL_NORMAL, int flags=SND_NOFLAGS, float volume=2.0, int pitch=SNDPITCH_NORMAL, int speakerentity=-1, const float origin[3]=NULL_VECTOR, const float dir[3]=NULL_VECTOR, bool updatePos=false, float soundtime=0.0);

/**
 * Called when a boss gets removed
 * 
 * @param client	Client index, however they may not always be in-game
 * 
 * @noreturn
 */
forward void FF2R_OnBossRemoved(int client);

/**
 * Wrapper to emit sound to one client.
 * 
 * @param client        Client index
 * @param key			Boss's sound key
 * @param boss			Boss's client index
 * @param required		Required value for a sound
 * @param entity        Entity to emit from
 * @param channel       Channel to emit with
 * @param level         Sound level
 * @param flags         Sound flags
 * @param volume        Sound volume
 * @param pitch         Sound pitch
 * @param speakerentity Unknown
 * @param origin        Sound origin
 * @param dir           Sound direction
 * @param updatePos     Unknown (updates positions?)
 * @param soundtime     Alternate time to play sound for
 * 
 * @error				Invalid client index
 * 
 * @return				True if the sound was found, false otherwise
 */
stock void FF2R_EmitBossSoundToClient(int client, const char[] key, int boss, const char[] required="", int entity=SOUND_FROM_PLAYER, int channel=SNDCHAN_AUTO, int level=SNDLEVEL_NORMAL, int flags=SND_NOFLAGS, float volume=2.0, int pitch=SNDPITCH_NORMAL, int speakerentity=-1, const float origin[3]=NULL_VECTOR, const float dir[3]=NULL_VECTOR, bool updatePos=false, float soundtime=0.0)
{
	int clients[1];
	clients[0] = client;
	
	/* Save some work for SDKTools and remove SOUND_FROM_PLAYER references */
	if(entity == SOUND_FROM_PLAYER)
		entity = client;
	
	return FF2R_EmitBossSound(clients, 1, key, boss, required, entity, channel, level, flags, volume, pitch, speakerentity, origin, dir, updatePos, soundtime);
}

/**
 * Wrapper to emit sound to all clients.
 * 
 * @param key			Boss's sound key
 * @param boss			Boss's client index
 * @param required		Required value for a sound
 * @param entity        Entity to emit from
 * @param channel       Channel to emit with
 * @param level         Sound level
 * @param flags         Sound flags
 * @param volume        Sound volume
 * @param pitch         Sound pitch
 * @param speakerentity Unknown
 * @param origin        Sound origin
 * @param dir           Sound direction
 * @param updatePos     Unknown (updates positions?)
 * @param soundtime     Alternate time to play sound for
 * 
 * @error				Invalid client index
 * 
 * @return				True if the sound was found, false otherwise
 */
stock void FF2R_EmitBossSoundToAll(const char[] key, int boss, const char[] required="", int entity=SOUND_FROM_PLAYER, int channel=SNDCHAN_AUTO, int level=SNDLEVEL_NORMAL, int flags=SND_NOFLAGS, float volume=2.0, int pitch=SNDPITCH_NORMAL, int speakerentity=-1, const float origin[3]=NULL_VECTOR, const float dir[3]=NULL_VECTOR, bool updatePos=false, float soundtime=0.0)
{
	int[] clients = new int[MaxClients];
	int total;
	
	for(int i=1; i<=MaxClients; i++)
	{
		if(IsClientInGame(i))
			clients[total++] = i;
	}
	
	return FF2R_EmitBossSound(clients, total, key, boss, required, entity, channel, level, flags, volume, pitch, speakerentity, origin, dir, updatePos, soundtime);
}


/**
 *	Below is related to using ConfigMap, though cfgmap is not required for this include, needed for the below
 */
#if defined CFGMAP_FF2R


/**
 * Get's the ConfigMap of the boss the player's playing as
 * 
 * @note			Don't close this handle
 * 
 * @param client	Client index
 * 
 * @error			Invalid client index
 * 
 * @return			ConfigMap handle if found, null otherwise
 */
native ConfigMap FF2R_GetBossData(int client);

/**
 * Changes primary handle the boss uses
 * 
 * @note			The previous handle the boss contains will be closed
 * 
 * @param client	Client index
 * @param cfg		ConfigMap handle, can be null to remove boss
 * 
 * @error			Invalid client index
 * 
 * @noreturn
 */
native void FF2R_SetBossData(int client, ConfigMap cfg=null);

methodmap ConfigData < ConfigMap
{
	public int GetInt(const char[] key, int defaul=0, int base=10)
	{
		int value = defaul;
		view_as<ConfigMap>(this).GetInt(key, value, base);
		return value;
	}
	public float GetFloat(const char[] key, float defaul=0.0)
	{
		float value = defaul;
		view_as<ConfigMap>(this).GetFloat(key, value);
		return value;
	}
	public int GetString(const char[] key, char[] buffer, int length, const char[] defaul="")
	{
		int size = view_as<ConfigMap>(this).Get(key, buffer, length);
		if(!size)
			size = strcopy(buffer, length, defaul);
		
		return size;
	}
	public bool GetBool(const char[] key, bool defaul=false)
	{
		bool value = defaul;
		view_as<ConfigMap>(this).GetBool(key, value, false);
		return value;
	}
	public void SetString(const char[] key, const char[] value)
	{
		view_as<ConfigMap>(this).Set(key, value);
	}
	public void SetBool(const char[] key, bool value)
	{
		view_as<ConfigMap>(this).SetInt(key, value ? 1 : 0);
	}
	public void Remove(const char[] key)
	{
		view_as<ConfigMap>(this).SetVal(key, _, -1);
	}
}

methodmap AbilityData < ConfigData
{
	public bool IsMyPlugin()
	{
		char plugin[64];
		if(this.GetString("plugin_name", plugin, sizeof(plugin))
		{
			char buffer[PLATFORM_MAX_PATH];
			GetPluginFilename(INVALID_HANDLE, buffer, sizeof(buffer));
			
			int highest = -1;
			for(int i = strlen(buffer)-1; i>0; i--)
			{
				if(buffer[i] == '/' || buffer[i] == '\\')
				{
					highest = i;
					break;
				}
			}
			
			if(!StrEqual(buffer[highest+1], plugin))
				return false;
		}
		return true;
	}
}

methodmap BossData < ConfigData
{
	public BossData(int client)
	{
		return view_as<BossData>(FF2R_GetBossData(client));
	}
	public AbilityData GetAbility(const char[] key)
	{
		return view_as<AbilityData>(view_as<ConfigMap>(this).GetSection(key));
	}
}

/**
 * Called when a boss gets created
 * 
 * @param client	Client index
 * @param cfg		Boss's ConfigMap
 * 
 * @noreturn
 */
forward void FF2R_OnBossCreated(int client, BossData cfg);

/**
 * Called when an ability is about to be activated
 * 
 * @param client	Client index
 * @param ability	Ability name
 * @param plugin	Plugin name
 * @param result	True if the ability will activate, false otherwise
 * @param cfg		Boss's ConfigMap of the ability
 * 
 * @return		Plugin_Changed if result was changed, Plugin_Handled to ignore FF2_PreAbility, Plugin_Stop to stop the ability
 */
forward Action FF2R_OnAbilityPre(int client, const char[] ability, const char[] plugin, AbilityData cfg, bool &result);

/**
 * Called when an ability is activated, only called if the ability has no plugin_name or named for your plugin
 * 
 * @param client	Client index
 * @param ability	Ability name
 * @param plugin	Plugin name
 * @param cfg		Boss's ConfigMap of the ability
 * 
 * @noreturn		
 */
forward void FF2R_OnAbility(int client, const char[] ability, const char[] plugin, AbilityData cfg);

/**
 * Called when after an ability was activated
 * 
 * @param client	Client index
 * @param ability	Ability name
 * @param plugin	Plugin name
 * @param cfg		Boss's ConfigMap of the ability
 * 
 * @noreturn		
 */
forward void FF2R_OnAbilityPost(int client, const char[] ability, const char[] plugin, AbilityData cfg);

#else

native void FF2R_SetBossData(int client, Handle cfg=null);
forward void FF2R_OnBossCreated(int client);
forward Action FF2R_OnAbilityPre(int client, const char[] ability, const char[] plugin, Handle cfg, bool &result);
forward void FF2R_OnAbility(int client, const char[] ability, const char[] plugin);
forward void FF2R_OnAbilityPost(int client, const char[] ability, const char[] plugin);

#endif

public SharedPlugin __pl_ff2r =
{
	name = "freak_fortress_2",
	file = "freak_fortress_2.smx",
	#if defined REQUIRE_PLUGIN
		required = 1,
	#else
		required = 0,
	#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ff2r_SetNTVOptional()
{
	MarkNativeAsOptional("FF2R_EmitBossSound");
	MarkNativeAsOptional("FF2R_GetBossData");
	MarkNativeAsOptional("FF2R_SetBossData");
}
#endif