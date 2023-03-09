#include "venus_plugin.h"

// match what you wish to display.

// Set UI for the "Send" screen. Does not set Title
static void set_sent_amount_eth_ui(ethQueryContractUI_t *msg) {

    const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
    uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

    // Converts the uint256 number located in `eth_amount` to its string representation and
    // copies this to `msg->msg`.
    amountToString(eth_amount, eth_amount_size, WEI_TO_ETHER, "ETH", msg->msg, msg->msgLength);
}

// Set UI for "Receive" screen. Does not set Title
static void set_received_amount_ui(ethQueryContractUI_t *msg, const context_t *context) {

    uint8_t decimals = context->decimals_received;
    const char *ticker = context->ticker_received;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token2_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount_received,
                   sizeof(context->amount_received),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}


// Set UI for "Beneficiary" screen. - Does not set title
static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary` and `getEthDisplayableAddress`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.

    getEthDisplayableAddress(
        context->beneficiary,
        msg->msg,
        msg->pluginSharedRW->sha3,
        chainid);
}

/* If we processed by screen 
// Set UI for First param screen
static void set_first_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case BEP20_APPROVE:
        case VENUS_MANUAL_VOTE:
            strlcpy(msg->title, "Proposal id.", msg->titleLength);
            amountToString(context->proposal_id,
                           sizeof(context->proposal_id),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case VENUS_VOTE_DELEGATE:
            strlcpy(msg->title, "Delegatee.", msg->titleLength);
            set_beneficiary_ui(msg, context);
            break;
    }
}
*/

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;
    
    // Process by selectorIndex

    // Like everything else - lets do it by selector - then by screen display, then call functions above to set the ui

    switch (context->selectorIndex) {

        // *** Bep20
        case BEP20_APPROVE:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Approve Venus.", msg->titleLength);
                    if (ismaxint(context->amount_sent, sizeof(context->amount_sent))) {
                        strlcpy(msg->msg, "Unlimited ", msg->msgLength);
                        strlcat(msg->msg, context->ticker_sent, msg->msgLength);
                    } else {
                        amountToString(context->amount_sent,
                                       sizeof(context->amount_sent),
                                       context->decimals_sent,
                                       context->ticker_sent,
                                       msg->msg,
                                       msg->msgLength);
                    }
                    break;
                case 1:
                    strlcpy(msg->title, "Venus spender.", msg->titleLength);
                    strlcpy(msg->msg, context->ticker_received, msg->msgLength);
                    break;
            }
            break;

        // *** Venus vTokens
        case VENUS_MINT:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Supply to Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_REDEEM:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Withdraw from Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_REDEEM_UNDERLYING:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Withdraw from Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_BORROW:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Borrow from Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_REPAY_BORROW:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Repay to Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_REPAY_BORROW_ON_BEHALF:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Repay to Venus.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
                case 1:
                    strlcpy(msg->title, "On behalf of.", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case VENUS_PROVIDE_COLLATERAL:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Enable collateral.", msg->titleLength);
                    strlcpy(msg->msg, context->ticker_sent, msg->msgLength);
                    break;
            }
            break;
            //msg->tokenLookup1 = context->contract_address_sent; TODO: when provide_parameter has passed address[] array

        case VENUS_REMOVE_COLLATERAL:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Disable collateral.", msg->titleLength);
                    strlcpy(msg->msg, context->ticker_sent, msg->msgLength);
                    break;
            }
            break;

        // *** Vaults ***
        case VAULT_DEPOSIT:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Stake.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;
        
        case VAULT_DEPOSIT_TOKEN:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Stake.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VAULT_WITHDRAW_TOKEN_EXECUTE:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Execute withdraw.", msg->titleLength);
                    break;
            }
            break;

        case VAULT_WITHDRAW_TOKEN_REQUEST:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Request withdraw.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;
            //msg->tokenLookup1 = context->contract_address_sent; // Reward Token
            break;

        case VAULT_WITHDRAW_VAI:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Withdraw stake.", msg->titleLength);
                    strlcpy(msg->msg, "from VAI vault", msg->msgLength);
                    break;
            }
            break;

        case VAULT_WITHDRAW_VRT:               
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Withdraw stake.", msg->titleLength);
                    strlcpy(msg->msg, "from VRT vault", msg->msgLength);
                    break;
            }
            break;

        case VAULT_CLAIM:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Claim reward.", msg->titleLength);
                    strlcpy(msg->msg, "from Venus vault", msg->msgLength);
                    break;
            }
            break;            

        // *** Governance ***
        case VENUS_DELEGATE_VOTE:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Delegate Vote.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
                case 1:
                    strlcpy(msg->title, "Delegate to.", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case VENUS_MAKE_PROPOSAL:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Create proposal.", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case VENUS_CAST_VOTE:
        case VENUS_VOTE_WITH_REASON:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Cast voote.", msg->titleLength);
                    // msg[0] = 'V', msg[1] = 'I', msg[2] = 'P', msg + 3    TODO:
                    amountToString(context->amount_sent,                    //Proposal ID
                                   sizeof(context->amount_sent),
                                   0,
                                   'VIP',
                                   msg->msg,
                                   msg->msgLength);
                    break;
                case 1:
                    strlcpy(msg->title, "Your vote", msg->titleLength);     //TODO:   
                    strlcpy(msg->msg, "for", msg->msgLength);               // Process SUPPORT param stored in context.amount_received
                    break;
            }
            break;

        // *** Convert ***
        case VENUS_CONVERT_VRT:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Convert to XVS.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        case VENUS_WITHDRAW_VESTED_XVS:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Withdraw XVS.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        // *** Swap ***
        case SWAP_EXACT_TOKENS_FOR_TOKENS:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send max.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive min.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case SWAP_TOKENS_FOR_EXACT_TOKENS:
            // msg->tokenLookup1 = context->contract_address_sent;
            // msg->tokenLookup2 = context->contract_address_received;
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send min.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive max.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case SWAP_EXACT_TOKENS_FOR_ETH:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send max.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive min.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case SWAP_TOKENS_FOR_EXACT_ETH:
            // msg->tokenLookup1 = context->contract_address_sent;
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send min.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive max.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case SWAP_EXACT_ETH_FOR_TOKENS:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send max.", msg->titleLength);
                    set_sent_amount_eth_ui(msg, context);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive min.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        case SWAP_ETH_FOR_EXACT_TOKENS:
            // msg->tokenLookup2 = context->contract_address_received;
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Swap send min.", msg->titleLength);
                    set_sent_amount_eth_ui(msg, context);
                    break;

                case 1:
                    strlcpy(msg->title, "Receive max.", msg->titleLength);
                    set_received_amount_ui(msg, context);
                    break;

                case 2: 
                    strlcpy(msg->title, "Beneficiary", msg->titleLength);
                    set_beneficiary_ui(msg, context);
                    break;
            }
            break;

        // *** VAI ***
        case VENUS_MINT_VAI:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Mint VAI.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;
            
        case VENUS_REPAY_VAI:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Repay VAI.", msg->titleLength);
                    amountToString(context->amount_sent,
                                   sizeof(context->amount_sent),
                                   context->decimals_sent,
                                   context->ticker_sent,
                                   msg->msg,
                                   msg->msgLength);
                    break;
            }
            break;

        // Keep this
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    /* Boilerplate example to process by screen index
    // Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            set_first_param_ui(msg);
            break;
        case 1:
            set_second_param_ui(msg, context);
            break;
        case 2:
            set_third_param_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    } */
}
