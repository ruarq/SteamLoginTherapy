#pragma once

static constexpr auto restart_steam_sh = R"(#!/bin/bash
killall steam &>/dev/null
pidof steam &>/dev/null
while [ $? -eq "0" ]
do
	sleep 0.1
	pidof steam &>/dev/null
done

steam &>/dev/null &
res = $?
disown $!
exit $res
)";
