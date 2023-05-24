// Experimental - do not use

import fetch from 'isomorphic-fetch';
import fs from 'fs';
import {utils} from 'ethers';

//import VaultAbi from './abis/venus_vault.json' assert { type: 'json' };
//import PoolAbi from './abis/venus_pool.json' assert { type: 'json' };

import VaultAbi from './networks/bnb-mainnet/venus/abis/venus_vault.json' assert { type: 'json' };
import PoolAbi from './networks/bnb-mainnet/venus/abis/venus_pool.json' assert { type: 'json' };
import vTokenAbi from './networks/bnb-mainnet/venus/abis/venus_vtokens.json' assert { type: 'json' };


const VAULTS_URL = 'https://api.venus.io/vaults' // Get list of contracts from Venus API server

const abisPath = './networks/bnb-mainnet/venus/abis/';
const b2cFile = './networks/bnb-mainnet/venus/b2c';
const contractsInfoFile = '../src/contracts-info.txt';

function b2cTemplate(chainId, contracts) {
  return {
    "chainId": chainId,
    "contracts": contracts,
    "name": "Venus"
  }
}

function b2cVaultTemplate(vault) {
  // address must be lowercase
  return {
    "address": vault.vaultAddress.toLowerCase(),
    "contractName": "Vault",
    "selectors": {
      "0xb6b55f25": {"erc20OfInterest": [],"method": "deposit","plugin": "Venus"},
      "0x4e71d92d": {"erc20OfInterest": [],"method": "claim","plugin": "Venus"}
    }
  }
}

function b2cPoolTemplate(vault) {
  // address must be lowercase
  return {
    "address": vault.rewardPool.toLowerCase(),
    "contractName": "Venus Pool",
    "selectors": {
      "0xa694fc3a": {"erc20OfInterest": [],"method": "stake","plugin": "Venus"},
      "0x3d18b912": {"erc20OfInterest": [],"method": "getReward","plugin": "Venus"},
      "0xe9fad8ee": {"erc20OfInterest": [],"method": "exit","plugin": "Venus"}
    }
  }
}

function b2cvTokenTemplate(vault) {
    // address must be lowercase
    return {
      "address": vault.vToken.toLowerCase(),
      "contractName": "vToken",
      "selectors": {
        "0xa0712d68": {"erc20OfInterest": [],"method": "mint","plugin": "Venus"},
        "0x852a12e3": {"erc20OfInterest": [],"method": "redeemUnderlying","plugin": "Venus"},
        "0xdb006a75": {"erc20OfInterest": [],"method": "redeem","plugin": "Venus"},
        "0x852a12e3": {"erc20OfInterest": [],"method": "redeemUnderlying","plugin": "Venus"},
        "0xc5ebeaec": {"erc20OfInterest": [],"method": "borrow","plugin": "Venus"},
        "0x0e752702": {"erc20OfInterest": [],"method": "repayBorrow","plugin": "Venus"}
      }
    }
  }
  
//We are writing this...

//#define _VENUS(address, underlying_ticker, underlying_decimals, vault_ticker, vault_decimals) \
//    {address, underlying_ticker, underlying_decimals, vault_ticker, vault_decimals},


function contractsInfoVaultTemplate(v) {
  // address must be mixed case checksum - underlying is not a v but we will put it in for a laugh for now
  return `_VENUS("${utils.getAddress(v.vaultAddress)}", "${v.id}", ${v.decimals}, "v${v.id}", 18)`;
}

function contractsInfoPoolTemplate(v) {
  // address must be mixed case checksum
  return `_VENUS("${utils.getAddress(v.rewardPool)}", "v${v.id}", 18, "", 18)`;
}

function contractsInfovTokenTemplate(v) {
  // address must be mixed case checksum
  //    return underlying Token and vToken- just remove the v char from the v.id for now
  return `_VENUS("${utils.getAddress(v.vToken)}", "${v.id.substring(1)}", ${v.decimals}, "${v.id}", 18)`;
}
  
async function fetchJson(url) {
  const response = await fetch(url);
  return response.json();
}

let contractsInfo = [];

async function updateContractsInfo(vaultsUrl) {
  const allVaults = await fetchJson(vaultsUrl);

   // await updateContractsForNetwork(1,   allVaults.eth);  // not using eth
  await updateContractsForNetwork(56,  allVaults.bsc);
  //  await updateContractsForNetwork(137, allVaults.matic); // not using polygon

  saveString(contractsInfoFile, contractsInfo.join('\n'));
}


async function updateContractsForNetwork(chainId, vaults) {
  const contracts = [];

  contractsInfo.push(`// ${chainId}`);

  for (const id in vaults) {
    const vault = vaults[id];

    if (vault.inactive) continue;

    saveContract(abisPath, vault.vaultAddress.toLowerCase(), VaultAbi);
    contracts.push(b2cVaultTemplate(vault));

    // if vault have rewardPool then add its contract
    contractsInfo.push(contractsInfoVaultTemplate(vault));

    if (vault.rewardPool) {
      saveContract(abisPath, vault.rewardPool.toLowerCase(), PoolAbi);
      contracts.push(b2cPoolTemplate(vault));
      contractsInfo.push(contractsInfoPoolTemplate(vault));
    }

    if (vault.vToken) {
        saveContract(abisPath, vault.vToken.toLowerCase(), vTokenAbi);
        contracts.push(b2cvTokenTemplate(vault));
        contractsInfo.push(contractsInfovTokenTemplate(vault));
      }
  }

  saveB2C(b2cFile, contracts, chainId);
}

function saveB2C(filename, contracts, chainId) {
  const b2c = b2cTemplate(chainId, contracts);
  const id = chainId === 1 ? '' : '.'+chainId;
  return saveObject(filename + id + '.json', b2c);
}

function saveContract(path, address, abi) {
  const filename = path + '/' + address + '.abi.json';
  return saveObject(filename, abi);
}

function saveObject(filename, obj) {
  return saveString(filename, JSON.stringify(obj, undefined, '\t'))
}

function saveString(filename, text) {
  fs.truncate(filename, 0, function() {
    return fs.writeFile(filename, text, err => {
      if (err) console.error(err);
      else console.log('saved', filename);
    });
  });
}

// Experimental do not use - updateContractsInfo(VAULTS_URL).then();