#include "venus_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);
    
    //assume success
    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {

        // *** Bep20
        case BEP20_APPROVE:
            strlcpy(msg->version, "Enable token", msg->versionLength);
            break;

        // *** Venus vTokens and vBnb***
        case VENUS_MINT_BNB:
        case VENUS_MINT:
            strlcpy(msg->version, "Supply", msg->versionLength);
            break;

        case VENUS_REDEEM:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;

        case VENUS_REDEEM_UNDERLYING:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;

        case VENUS_BORROW:
            strlcpy(msg->version, "Borrow", msg->versionLength);
            break;

        case VENUS_REPAY_BORROW:
        case VENUS_REPAY_BORROW_BNB:
            strlcpy(msg->version, "Repay", msg->versionLength);
            break;

        case VENUS_REPAY_BORROW_ON_BEHALF:
            strlcpy(msg->version, "Repay", msg->versionLength);
            break;

        // *** Comptroller ***
        case VENUS_PROVIDE_COLLATERAL:
            strlcpy(msg->version, "Enable collateral", msg->versionLength);
            break;

        case VENUS_REMOVE_COLLATERAL:
            strlcpy(msg->version, "Disable collateral", msg->versionLength);
            break;

        // *** Vaults ***
        case VAULT_DEPOSIT:
            //using ticker of token being deposited for vault name  eg. VAI or VRT
            strlcpy(msg->version, context->ticker_sent, msg->versionLength); 
            strlcat(msg->version, " vault", msg->versionLength); 
            break;
        
        case VAULT_DEPOSIT_TOKEN:
            strlcpy(msg->version, "XVS vault", msg->versionLength);
            break;

        case VAULT_WITHDRAW_TOKEN_EXECUTE:
            strlcpy(msg->version, "XVS vault", msg->versionLength);
            break;

        case VAULT_WITHDRAW_TOKEN_REQUEST:
            strlcpy(msg->version, "XVS vault", msg->versionLength);
            break;

        case VAULT_WITHDRAW_VAI:
            strlcpy(msg->version, "VAI vault", msg->versionLength);
            break;

        case VAULT_WITHDRAW_VRTXVS:
            strlcpy(msg->version, context->ticker_sent, msg->versionLength); 
            strlcat(msg->version, " vault withdraw", msg->versionLength); 
            break;

        case VAULT_CLAIM:
            strlcpy(msg->version, context->ticker_sent, msg->versionLength); 
            strlcat(msg->version, " vault", msg->versionLength); 
            /*TODO display VAI or VRT before "vault" - but there is no way we know which one
                   as the method is the same for each vault and has no paramters eg. claim()
                   Removed from spec for now 
            */
            break;

        // *** Governance ***
        case VENUS_DELEGATE_VOTE:
            strlcpy(msg->version, "Delegate vot. power", msg->versionLength);
            break;

        case VENUS_MAKE_PROPOSAL:
            strlcpy(msg->version, "Create proposal", msg->versionLength);
            break;

        case VENUS_CAST_VOTE:
        case VENUS_VOTE_WITH_REASON:
            strlcpy(msg->version, "Vote", msg->versionLength);
            break;

        // *** Convert ***
        case VENUS_CONVERT_VRT:
            strlcpy(msg->version, "Convert VRT to XVS", msg->versionLength);
            break;

        // *** Swap
        case SWAP_EXACT_TOKENS_FOR_TOKENS:
        case SWAP_EXACT_ETH_FOR_TOKENS:
        case SWAP_EXACT_TOKENS_FOR_ETH:
        case SWAP_TOKENS_FOR_EXACT_TOKENS:
        case SWAP_ETH_FOR_EXACT_TOKENS:
        case SWAP_TOKENS_FOR_EXACT_ETH:
            strlcpy(msg->version, "Swap", msg->versionLength);
            break;

        // *** VAI
        case VENUS_MINT_VAI:
            strlcpy(msg->version, "Mint VAI", msg->versionLength);
            break;
 
        case VENUS_REPAY_VAI:        
            strlcpy(msg->version, "Repay VAI", msg->versionLength);
            break;

        // Keep this
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}