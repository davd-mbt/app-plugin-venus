#include "venus_plugin.h"

static void copy_text(uint8_t *dst, uint16_t dst_len, uint16_t max_len, uint8_t *src) {
    size_t len = MIN(dst_len, max_len);
    memcpy(dst, src, len);
}

// One param functions handler
static void handle_one_param_function(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    // Context->  amount_sent is the default display for any thing that is an "amount"
    switch (context->next_param) {
        case MINT_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case REDEEM_TOKENS:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case REDEEM_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case BORROW_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case REPAY_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case COLLATERAL_TOKENS: //TODO Its an Array - but just use vTokens[1]
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contact_address_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case COLLATERAL_VTOKEN:
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contact_address_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case DEPOSIT_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case WITHDRAW_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case DELEGATEE:
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        
        case VRT_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case VAI_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Bep20 approve
static void handle_approve(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case SPENDER:
            copy_address(context->contract_address_received, msg->parameter, sizeof(context->contract_address_received));
            context->next_param = AMOUNT_APPROVED;
            break;

        case AMOUNT_APPROVED:
            copy_parameter(context->amount_sent,
                           msg->parameter,
                           sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Repay borrow on behalf handler
static void repay_borrow_on_behalf(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    //TODO - we need to get the Payable AMOUNT - should be the first parameter
    //          but looking at the transaction, only 2 parameters are passed

    switch (context->next_param) {
        case BORROWER: 
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = VBNB_TOKEN;
            break;
        case VBNB_TOKEN:
            copy_address(context->contact_address_sent, msg->parameter, sizeof(context->contact_address_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// *** Vault ***

static void handle_vault_token_function(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    switch (context->next_param) {
        case REWARD_TOKEN: 
            copy_address(context->contact_address_sent, msg->parameter, sizeof(context->contract_address_sent));
            context->next_param = VAULT_PID;
            break;
        case VAULT_PID: // store PID in amount_received - use to locate name of vault later maybe
            copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->next_param = REWARD_TOKEN_AMOUNT;
            break;
        case REWARD_TOKEN_AMOUNT:
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// *** Goverance

// Cast vote
static void handle_cast_vote(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    // Proposal ID and Support has to be shown using amount_sent and amount_received
    switch (context->next_param) {
        case PROPOSAL_ID:  // PROPOSAl_ID
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = SUPPORT;
            break;
        case SUPPORT:
            copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Vote with Reason
static void handle_vote_with_reason(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    // Proposal ID and Support has to be shown using amount_sent and amount_received
    switch (context->next_param) {
        case PROPOSAL_ID:  // PROPOSAl_ID
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = SUPPORT;
            break;

        case SUPPORT:
            copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->next_param = REASON;
            break;

        case REASON:
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// *** SWAP FUNCTIONS ***

// Copy amount sent parameter to amount_sent
static void handle_amount_sent(const ethPluginProvideParameter_t *msg,
                               context_t *context) {
    copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
}

// Copy amount sent parameter to amount_received
static void handle_amount_received(const ethPluginProvideParameter_t *msg,
                                   context_t *context) {
    copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
}

static void handle_beneficiary(const ethPluginProvideParameter_t *msg,
                               context_t *context) {
    copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_array_len(const ethPluginProvideParameter_t *msg,
                             context_t *context) {
    context->array_len = msg->parameter[PARAMETER_LENGTH - 1];
    PRINTF("LIST LEN: %d\n", context->array_len);
}

static void handle_token_sent(const ethPluginProvideParameter_t *msg,
                              context_t *context) {
    copy_address(context->contact_address_sent, msg->parameter, sizeof(context->contract_address_sent));
    PRINTF("TOKEN SENT: %.*H\n", ADDRESS_LENGTH, context->contract_address_sent);
}

static void handle_token_received(const ethPluginProvideParameter_t *msg,
                                  context_t *context) {
    copy_address(context->contract_address_received, msg->parameter, sizeof(context->contract_address_received));
    PRINTF("TOKEN RECEIVED: %.*H\n", ADDRESS_LENGTH, context->contract_address_received);
}


// Copy amount sent parameter to amount_sent
static void handle_value_sent(const ethPluginProvideParameter_t *msg,
                              context_t *context) {
    ethPluginSharedRO_t *pluginSharedRO = msg->pluginSharedRO;

    copy_parameter(context->amount_sent,
                   pluginSharedRO->txContent->value.value,
                   pluginSharedRO->txContent->value.length);
}

static void handle_swap_exact_tokens(ethPluginProvideParameter_t *msg,
                                     context_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // amountIn
            handle_amount_sent(msg, context);
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // amountOut
            handle_amount_received(msg, context);
            context->next_param = PATHS_OFFSET;
            break;
        case PATHS_OFFSET:
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            handle_beneficiary(msg, context);
            context->next_param = PATH;
            context->skip++;
            break;
        case PATH:  // len(path)
            handle_array_len(msg, context);
            context->next_param = TOKEN_SENT;
            break;
        case TOKEN_SENT:  // path[0]
            handle_token_sent(msg, context);
            context->next_param = TOKEN_RECEIVED;
            break;
        case TOKEN_RECEIVED:  // path[len(path) - 1]
            handle_token_received(msg, context);
            context->next_param = NONE;
            break;

        case NONE:
            break;

        default:
            PRINTF("Unsupported param\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_tokens_for_exact_tokens(ethPluginProvideParameter_t *msg,
                                                context_t *context) {
    switch (context->next_param) {
        case AMOUNT_RECEIVED:  // amountOut
            handle_amount_received(msg, context);

            if (context->selectorIndex == SWAP_EXACT_ETH_FOR_TOKENS ||
                context->selectorIndex == SWAP_ETH_FOR_EXACT_TOKENS ) {
                handle_value_sent(msg, context);
                context->next_param = PATHS_OFFSET;
            } else {
                context->next_param = AMOUNT_SENT;
            }
            break;

        case AMOUNT_SENT:  // amountIn
            handle_amount_sent(msg, context);
            context->next_param = PATHS_OFFSET;
            break;

        case PATHS_OFFSET:
            context->next_param = BENEFICIARY;
            break;

        case BENEFICIARY:  // to
            handle_beneficiary(msg, context);
            context->next_param = PATH;
            context->skip++;
            break;

        case PATH:  // len(path)
            context->next_param = TOKEN_SENT;
            break;

        case TOKEN_SENT:  // path[0]
            handle_token_sent(msg, context);
            context->next_param = TOKEN_RECEIVED;
            break;

        case TOKEN_RECEIVED:  // path[len(path) - 1]
            handle_token_received(msg, context);
            context->next_param = NONE;
            break;

        case NONE:
            break;

        default:
            PRINTF("Unsupported param\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Boilerplate example - just for reference
static void handle_swap_exact_eth_for_tokens(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case AMOUNT_RECEIVED:  // amountOutMin
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = PATHS_OFFSET;
            break;
        case PATHS_OFFSET:  // path
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = PATHS_LENGTH;
            context->go_to_offset = true;
            break;
        case PATHS_LENGTH:
            context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH * 2;
            context->go_to_offset = true;
            context->next_param = TOKEN_RECEIVED;
            break;
        case TOKEN_RECEIVED:  // path[1] -> contract address of token received
            copy_address(context->contract_address_received, msg->parameter, sizeof(context->contract_address_received));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}


void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (context->skip) {
        // Skip this step, and don't forget to decrease skipping counter.
        context->skip--;
    } else {
        if ((context->offset) && msg->parameterOffset != context->checkpoint + context->offset) {
            PRINTF("offset: %d, checkpoint: %d, parameterOffset: %d\n",
                   context->offset,
                   context->checkpoint,
                   msg->parameterOffset);
            return;
        }
        context->offset = 0;  // Reset offset

    // EDIT THIS: adapt the cases and the names of the functions.

        switch (context->selectorIndex) {

            // *** Bep20
            case BEP20_APPROVE:
                handle_approve(msg, context);        
                break;

            // *** Venus vTokens
            case VENUS_MINT:
            case VENUS_REDEEM:
            case VENUS_REDEEM_UNDERLYING:
            case VENUS_BORROW:
            case VENUS_REPAY_BORROW:
                handle_one_param_function(msg, context);
                break;

            case VENUS_REPAY_BORROW_ON_BEHALF:
                repay_borrow_on_behalf(msg, context);
                break;
            
            //  *** Comptroller
            case VENUS_PROVIDE_COLLATERAL:
            case VENUS_REMOVE_COLLATERAL:
                handle_one_param_function(msg, context);
                break;

            // *** Vaults ***
            case VAULT_DEPOSIT:  // TODO: Supply name of Vault
            case VAULT_WITHDRAW_VAI:
            case VAULT_WITHDRAW_VRT:
                handle_one_param_function(msg, context);
                break;

            case VAULT_DEPOSIT_TOKEN: 
            case VAULT_WITHDRAW_TOKEN_REQUEST:
            case VAULT_WITHDRAW_EXECUTE:
                handle_vault_token_function(msg, context);
                break;

            case VAULT_CLAIM:   // TODO: Supply name of Vault
                //No parameters to handle
                break;

            // *** Governance ***
            case VENUS_VOTE_DELEGATE:
                handle_one_param_function(msg, context);
                break;

            case VENUS_MAKE_PROPOSAL:
                // handle_make_proposal_function(msg, context); TODO: Extract Title from JSON in DESCRIPTION
                break;   

            case VENUS_CAST_VOTE:
                handle_cast_vote(msg, context);
                break;

            case VENUS_VOTE_WITH_REASON:
                handle_vote_with_reason(msg, context);
                break;

            // *** Convert ***
            case VENUS_CONVERT_VRT:
                handle_one_param_function(msg, context);
                break;
    
            case VENUS_WITHDRAW_VESTED_XVS:
                //No parameters to handle
                break;

            // *** Swap ***
            case SWAP_EXACT_TOKENS_FOR_TOKENS:
            case SWAP_EXACT_TOKENS_FOR_ETH:
                handle_swap_exact_tokens(msg, context);
                break;

            case SWAP_EXACT_ETH_FOR_TOKENS:
            case SWAP_ETH_FOR_EXACT_TOKENS:
            case SWAP_TOKENS_FOR_EXACT_TOKENS:
            case SWAP_TOKENS_FOR_EXACT_ETH:
                handle_swap_tokens_for_exact_tokens(msg, context);
                break;

            // *** VAI ***
           case VENUS_MINT_VAI:
           case VENUS_REPAY_VAI:
                handle_one_param_function(msg, context);
                break;

            default:
                PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
        
    }
}