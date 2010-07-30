/* OperServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandOSMode : public Command
{
 public:
	CommandOSMode() : Command("MODE", 2, 2, "operserv/mode")
	{
	}

	CommandReturn Execute(User *u, const std::vector<Anope::string> &params)
	{
		Anope::string chan = params[0], modes = params[1];
		Channel *c;

		if (!(c = findchan(chan)))
			notice_lang(Config.s_OperServ, u, CHAN_X_NOT_IN_USE, chan.c_str());
		else if (c->bouncy_modes)
			notice_lang(Config.s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
		else
		{
			c->SetModes(OperServ, false, modes.c_str());

			if (Config.WallOSMode)
				ircdproto->SendGlobops(OperServ, "%s used MODE %s on %s", u->nick.c_str(), modes.c_str(), chan.c_str());
		}
		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &subcommand)
	{
		notice_help(Config.s_OperServ, u, OPER_HELP_MODE);
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &subcommand)
	{
		syntax_error(Config.s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		notice_lang(Config.s_OperServ, u, OPER_HELP_CMD_MODE);
	}
};

class OSMode : public Module
{
 public:
	OSMode(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		this->AddCommand(OperServ, new CommandOSMode());
	}
};

MODULE_INIT(OSMode)