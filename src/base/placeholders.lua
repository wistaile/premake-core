--
-- placeholders.lua
-- Copyright (c) 2017
--

function global(name)
	printf("global '%s'", name)
end

function workspace(name)
	printf("workspace '%s'", name)
end

function solution(name)
	printf("solution '%s'", name)
end

function project(name)
	printf("project '%s'", name)
end

function group(name)
	printf("group '%s'", name)
end

function configuration(terms)
	printf("configuration " .. premake.valueToCode(terms))
end

function filter(terms)
	print("filter " .. premake.valueToCode(terms))
end
