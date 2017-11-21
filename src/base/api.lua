--
-- api.lua
-- Copyright (c) 2017
--

	local p = premake
	p.api = {}

	local api = p.api


	function api.register(field)
		local name = field.name
		if not name then
			error("missing name", 2)
		end

		if rawget(_G, name) then
			error("name '" .. name .. "' in use", 2)
		end

		api.n_register(field)

		_G[name] = function(value)
			printf("    %s %s", name, p.valueToCode(value))
			api.n_add(name, value)
		end

		-- TODO, not all fields get a 'remove' variant.
		_G["remove" .. name] = function(value)
			printf("    remove%s %s", name, p.valueToCode(value))
			api.n_remove(name, value)
		end

		return field
	end

	function api.alias(original, alias)
		p.alias(_G, original, alias)
		if _G["remove" .. original] then
			p.alias(_G, "remove" .. original, "remove" .. alias)
		end
	end

	function api.addAllowed(fieldName, value)
	end

	function api.deprecateField(name, message, handler)
	end

	function api.deprecateValue(name, value, message, addHandler, removeHandler)
	end

	function api.deprecations(value)
	end

	p.n_initNativeApi()
