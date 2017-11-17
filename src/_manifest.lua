--
-- _manifest.lua
-- Manage the list of built-in Premake scripts.
-- Copyright (c) 2002-2015 Jason Perkins and the Premake project
--

-- The master list of built-in scripts. Order is important! If you want to
-- build a new script into Premake, add it to this list.

	return
	{
		-- core files
		"base/_foundation.lua",
		"base/action.lua",
		"base/api.lua",
		"base/globals.lua",
		"base/help.lua",
		"base/http.lua",
		"base/io.lua",
		"base/json.lua",
		"base/jsonwrapper.lua",
		"base/moduledownloader.lua",
		"base/option.lua",
		"base/os.lua",
		"base/path.lua",
		"base/semver.lua",
		"base/string.lua",
		"base/table.lua",
		"base/term.lua",
		"base/tools.lua",

		"base/placeholders.lua",

		"base/premake.lua",
		"_premake_init.lua",
	}
