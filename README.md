# app-plugin-venus

This repo contains a ledger plugin for the Venus Protocol smart contract on BnbChain.

Plugins are lightweight applications that go hand-in-hand with the Ethereum
Application on a Nano S, SP, X and Stax device.

They allow users to safely interact with smart contracts by parsing the
transaction data and displaying its content in a human readable way. This is
done on a "per contract" basis, meaning a plugin is required for every DApp.

## Developer Contributors and Attribution Notice for app-plugin-venus

app-plugin-venus was built by Dav Dumbledore of ClearSign Technology from the forkable app-plugin-boilerplate following the ledger 
[plugin guide](https://developers.ledger.com/docs/dapp/embedded-plugin/overview/)

Inspired by ideas and code samples found in other open source app-plugin project repo's hosted on GitGub for [LedgerHQ](https://github.com/LedgerHQ)

Dav Dumbledore can be reached via:
Twitter X: @Davd_MBT 
Github: [davd-mbt](https://github.com/davd-mbt/), 
Website [ClearSign Technology](https://clearsign.tech) 

## Ethereum SDK

Ethereum/BnbChain plugins need the [Ethereum SDK](https://github.com/LedgerHQ/ethereum-plugin-sdk).
You can use the `ETHEREUM_PLUGIN_SDK` variable to point to the directory where you cloned this repository. By default, the `Makefile` expects it to be at the root directory of this plugin repository, by the `ethereum-plugin-sdk` name.

This repository is deliberately **not** a submodule. You can see that the CI workflows clone and checkout either the latest `master` or on `develop` references. This ensures the code is compiled and tested on the latest version of the SDK.

## Smart Contracts

Venus Protocol smart contracts on the Bnb Smart Chain covered by this plugin are:

| Contract Name          | Smart Contract                               |
| ---------------------- | -------------------------------------------- |
| VBNB                   | `0xa07c5b74c9b40447a954e1466938b865b6bbea36` |
| Maximillion            | `0x5efa1e46f4fd738ff721f5aebc895b970f13e8a1` |
| Unitroller             | `0xfd36e2c2a6789db23113685031d7f16329158384` |
| VAIVaultProxy          | `0x0667eed0a0aab930af74a3dfedd263a73994f216` |
| VRTVaultProxy          | `0x98bf4786d72aaef6c714425126dd92f149e3f334` |
| XVSVaultProxy          | `0x051100480289e704d20e9db4804837068f3f9204` |
| GovernorBravoDelegator | `0x2d56dc077072b53571b8252008c60e945108c75a` |
| VRTConverterProxy      | `0x92572fb60f4874d37917c53599cae5b085b9facd` |
| XVSVestingProxy        | `0xb28dec7c7ac80f4d0b6a1b711c39e444cde8b2ce` |
| PancakeRouter          | `0x17f4a746a7bf05c3e24a2bb7d7d25e4d3e5bbe3e` |
| VAIUnitroller          | `0x004065d34c6b18ce4370ced1cebde94865dbfafe` |


This plugin also covers Venus vTokens contracts created from the principle vBep20Delegator contract including:

| vToken                 | Smart Contract                               |
| ---------------------- | -------------------------------------------- |
| vUSDT                  | `0xfd5840cd36d94d7229439859c0112a4185bc0255` |
| vUSDC                  | `0xecA88125a5ADbe82614ffC12D0DB554E2e2867C8` |
| vBUSD                  | `0x95c78222B3D6e262426483D42CfA53685A67Ab9D` |
| vXVS                   | `0x151B1e2635A717bcDc836ECd6FbB62B674FE3E1D` |
| vBTC                   | `0x882C173bC7Ff3b7786CA16dfeD3DFFfb9Ee7847B` |
| vETH                   | `0xf508fCD89b8bd15579dc79A6827cB4686A3592c8` |
| vLTC                   | `0x57A5297F2cB2c0AaC9D554660acd6D385Ab50c6B` |
| vXRP                   | `0xB248a295732e0225acd3337607cc01068e3b9c10` |
| vBCH                   | `0x5F0388EBc2B94FA8E123F404b79cCF5f40b29176` |

This plugin also covers the approval method of the Bep20 token contracts underlying each Venus vToken