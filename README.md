# app-plugin-venus

This repo contains a ledger plugin for the Venus Protocol smart contract on BnbChain.

Plugins are lightweight applications that go hand-in-hand with the Ethereum
Application on a Nano S / X device.

They allow users to safely interact with smart contracts by parsing the
transaction data and displaying its content in a human readable way. This is
done on a "per contract" basis, meaning a plugin is required for every DApp.

app-plugin-venus was built from the forkable app-plugin-boilerplate following the 
[plugin guide](https://developers.ledger.com/docs/dapp/embedded-plugin/overview/)

It is STRONGLY recommended to follow the guide in order to better understand the flow and the context for plugins.

## Developer Contact for app-plugin-venus

Dav Dumbledore follow on Twitter: @Davd_MBT

## Ethereum SDK

Ethereum/BnbChain plugins need the [Ethereum SDK](https://github.com/LedgerHQ/ethereum-plugin-sdk).
You can use the `ETHEREUM_PLUGIN_SDK` variable to point to the directory where you cloned
this repository. By default, the `Makefile` expects it to be at the root directory of this
plugin repository, by the `ethereum-plugin-sdk` name.

This repository is deliberately **not** a submodule. You can see that the CI workflows
clone and checkout either the latest `master` or on `develop` references. This ensures
the code is compiled and tested on the latest version of the SDK.

## Formatting

The C source code is expected to be formatted with `clang-format` 11.0.0 or higher.
