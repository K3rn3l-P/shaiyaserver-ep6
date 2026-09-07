# Shaiya Episode 6 (archived fork)

Fork of [kurtekat/shaiya-episode-6](https://github.com/kurtekat/shaiya-episode-6), a server-side
DLL injection framework for episode 6 Shaiya private servers (client, login, game and db
processes). Archived: dormant since August 2025, no active development.

## What this adds

Refactored a large part of the packet-handling code from free functions into namespaced modules,
ported parts of it to C++20 (`<ranges>`), and reorganized the reverse-engineered client headers
under a single `include/shaiya/include/` tree. Custom packet handlers cover trading, party,
mailbox, market, personal shop, quests and item rewards.

This is also where the `/mmake`, `/giveitem` and `/mera` custom server commands referenced in
[PSM_Cmd-SecureCommandChannel](https://github.com/K3rn3l-P/PSM_Cmd-SecureCommandChannel) come
from.

## Environment

Windows 10, Visual Studio 2022, C++23.

## Attribution

The base injection framework, reverse-engineered client structures and build/injection notes are
[kurtekat/shaiya-episode-6](https://github.com/kurtekat/shaiya-episode-6), shared as-is by the
author, no license attached.

## State

Archived, no further changes planned.
