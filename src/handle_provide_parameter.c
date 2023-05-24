#include "venus_plugin.h"

static void copy_text(uint8_t *dst, uint16_t dst_len, uint16_t max_len, uint8_t *src) {
    size_t len = MIN(dst_len, max_len);
    memcpy(dst, src, len);
}


// No Parameters are expected on some selectors, return error just in case params are passed
static void handle_no_param_function(ethPluginProvideParameter_t *msg, context_t *context) {
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
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

        case COLLATERAL_VTOKEN:
            memset(context->contract_address_sent, 0, sizeof(context->contract_address_sent));
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
            //memcpy(context->contract_address_sent, &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],ADDRESS_LENGTH);
            printf_hex_array("Disable vToken: ", ADDRESS_LENGTH, context->contract_address_sent);

            /* copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
            PRINTF("Collateral vToken Address: %s\n", context->contract_address_sent);*/
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
static void handle_approve_function(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case APPROVE_SPENDER:
            copy_address(context->contract_address_received, msg->parameter, sizeof(context->contract_address_received));
            context->next_param = APPROVE_AMOUNT;
            break;

        case APPROVE_AMOUNT:
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
static void handle_repay_borrow_on_behalf_function(ethPluginProvideParameter_t *msg, context_t *context) {
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
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// handle collateral tokens

static void handle_collateral_tokens_function(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case COLLATERAL_TOKENS_OFFSET:
            context->skip++; // skips next parameter which is the array length
            context->next_param = COLLATERAL_TOKEN;
            break;

        case COLLATERAL_TOKEN: // Using 1st element in array to get token
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
            PRINTF("Collateral vToken Address: %s\n", context->contract_address_sent);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
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
            copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
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
static void handle_cast_vote_function(ethPluginProvideParameter_t *msg, context_t *context) {
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
            context->decimals_sent = msg->parameter[PARAMETER_LENGTH-1];
            //copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Vote with Reason
static void handle_vote_with_reason_function(ethPluginProvideParameter_t *msg, context_t *context) {
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
            context->decimals_sent = msg->parameter[PARAMETER_LENGTH-1];  // last byte contains a single digit 0,1 or 2
            //copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->skip++; //skip offset
            context->next_param = REASON_LEN;
            break;

        case REASON_LEN:        // this parameter is length of string for reason
            context->next_param = REASON_DESCRIPTION;
            break;

        case REASON_DESCRIPTION:        // copy parameter to the largest context field we have amount_received is 32 bytes
            copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            context->next_param = NONE;
            break;

        case NONE:           // igrnore further parameters
            break;

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// *** SWAP FUNCTIONS ***

// Copy amount sent parameter to amount_sent
static void handle_amount_sent_function(const ethPluginProvideParameter_t *msg,
                               context_t *context) {
    copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
}

// Copy amount sent parameter to amount_received
static void handle_amount_received_function(const ethPluginProvideParameter_t *msg,
                                   context_t *context) {
    copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
}

static void handle_beneficiary_function(const ethPluginProvideParameter_t *msg,
                               context_t *context) {
    copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary); // Print Bytes
}

static void handle_array_len_function(const ethPluginProvideParameter_t *msg,
                             context_t *context) {
    //context->array_len = msg->parameter[PARAMETER_LENGTH - 1];  // 1 byte gives maz value of 255
    context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2); //using offset 2 bytes max value 65535
    PRINTF("LIST LEN: %d\n", context->offset);
}

static void handle_token_sent_function(const ethPluginProvideParameter_t *msg,
                              context_t *context) {
    copy_address(context->contract_address_sent, msg->parameter, sizeof(context->contract_address_sent));
    PRINTF("TOKEN SENT: %.*H\n", ADDRESS_LENGTH, context->contract_address_sent); // Print Bytes
}

static void handle_token_received_function(const ethPluginProvideParameter_t *msg,
                                  context_t *context) {
    copy_address(context->contract_address_received, msg->parameter, sizeof(context->contract_address_received));
    PRINTF("TOKEN RECEIVED: %.*H\n", ADDRESS_LENGTH, context->contract_address_received); // Print Bytes
}


// Copy amount sent parameter to amount_sent
static void handle_value_sent_function(const ethPluginProvideParameter_t *msg,
                              context_t *context) {
    ethPluginSharedRO_t *pluginSharedRO = msg->pluginSharedRO;

    copy_parameter(context->amount_sent,
                   pluginSharedRO->txContent->value.value,
                   pluginSharedRO->txContent->value.length);
    PRINTF("Amount Sent: %d\n", context->amount_sent);
}

static void handle_swap_exact_tokens_function(ethPluginProvideParameter_t *msg,
                                     context_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // amountIn
            handle_amount_sent_function(msg, context);
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // amountOut
            handle_amount_received_function(msg, context);
            context->next_param = PATHS_OFFSET;
            break;
        case PATHS_OFFSET:
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            handle_beneficiary_function(msg, context);
            context->next_param = PATH;
            context->skip++; // skip field 'deadline'
            break;
        case PATH:  // len(path)
            handle_array_len_function(msg, context); // sets content->offset to array length eg. 2 or 3
            context->next_param = TOKEN_SENT;
            break;
        case TOKEN_SENT:  // path[0] process 1st array element as token sent
            handle_token_sent_function(msg, context);
            context->next_param = TOKEN_RECEIVED;

            if (context->offset > 2) context->skip = context->offset -2; //skip to end of array if we have more than 2 tokens
            PRINTF("Skipping next: %d parameters\n", context->skip);
            context->offset = 0;
            break;
        case TOKEN_RECEIVED:  // path[len(path) - 1]
            handle_token_received_function(msg, context);
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

static void handle_swap_tokens_for_exact_tokens_function(ethPluginProvideParameter_t *msg,
                                                context_t *context) {
    switch (context->next_param) {
        case AMOUNT_RECEIVED:  // amountOut
            handle_amount_received_function(msg, context);

            if (context->selectorIndex == SWAP_EXACT_ETH_FOR_TOKENS ||
                context->selectorIndex == SWAP_ETH_FOR_EXACT_TOKENS ) {
                handle_value_sent_function(msg, context);
                context->next_param = PATHS_OFFSET;
            } else {
                context->next_param = AMOUNT_SENT;
            }
            break;

        case AMOUNT_SENT:  // amountIn
            handle_amount_sent_function(msg, context);
            context->next_param = PATHS_OFFSET;
            break;

        case PATHS_OFFSET:
            context->next_param = BENEFICIARY;
            break;

        case BENEFICIARY:  // to
            handle_beneficiary_function(msg, context);
            context->next_param = PATH;
            context->skip++; //skip next parameter 'deadline'
            break;

        case PATH:  // len(path)
            handle_array_len_function(msg, context); // sets content->offset to array length eg. 2 or 3
            context->next_param = TOKEN_SENT;
            break;

        case TOKEN_SENT:  // path[0]
            handle_token_sent_function(msg, context);
            context->next_param = TOKEN_RECEIVED;

            if (context->offset > 2) context->skip = context->offset -2; //skip to end of array if we have more than 2 tokens
            PRINTF("Skipping next: %d parameters\n", context->skip);
            context->offset = 0;
            break;

        case TOKEN_RECEIVED:  // path[len(path) - 1]
            handle_token_received_function(msg, context);
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

static void handle_make_proposal_function(ethPluginProvideParameter_t *msg,
                                                context_t *context) {

    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    switch (context->next_param) {
        case PROPOSAL_TARGETS_OFFSET:  // address[]
            context->next_param = PROPOSAL_VALUES_OFFSET;
            break;

        case PROPOSAL_VALUES_OFFSET:  // uint256[]
            context->next_param = PROPOSAL_SIGNATURES_OFFSET;
            break;

        case PROPOSAL_SIGNATURES_OFFSET:  // strings[]
            context->next_param = PROPOSAL_CALLDATAS_OFFSET;
            break;

        case PROPOSAL_CALLDATAS_OFFSET:  // bytes[]
            context->next_param = PROPOSAL_DESCRIPTION_OFFSET;
            break;

        case PROPOSAL_DESCRIPTION_OFFSET:  // string
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->next_param = PROPOSAL_TYPE;
            break;

        case PROPOSAL_TYPE:  // uint8
            context->next_param = PROPOSAL_DESCRIPTION_LEN;
            context->go_to_offset = true; //offset of proposal_description set in previous param
            PRINTF("Go to offset set as: %d\n", context->offset);
            break;

        case PROPOSAL_DESCRIPTION_LEN:  // last 2 bytes gives number of parameters for description string[]
            handle_array_len_function(msg, context);
            if (context->offset >= 1) {
                context->next_param = PROPOSAL_DESCRIPTION_TEXT_1;
                }
            else { context->next_param = NONE;
            }
            context->decimals_sent = 0; // For number of screens
            break;

        case PROPOSAL_DESCRIPTION_TEXT_1:  // string
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            // each param is 32 bytes - the context address fields are 20 bytes, amount fields 32 - oh be joyful
            if (context->offset >= 2) {
                context->next_param = PROPOSAL_DESCRIPTION_TEXT_2;
                }
            else { context->next_param = NONE;
            }
            context->decimals_sent++;
            PRINTF("PROPOSAL_1: %s\n", context->amount_sent);
            break;

        case PROPOSAL_DESCRIPTION_TEXT_2:  // string
            copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
            if (context->offset >= 3) {
                context->next_param = PROPOSAL_DESCRIPTION_TEXT_3;
                }
            else { context->next_param = NONE;
            }
            context->decimals_sent++;
            PRINTF("PROPOSAL_2: %s\n", context->amount_received);
            break;

        case PROPOSAL_DESCRIPTION_TEXT_3:  // string
            copy_parameter(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = NONE;
            context->decimals_sent++;
            PRINTF("PROPOSAL_3: %s\n", context->beneficiary);
            break;



/*
        case PATHS_OFFSET:
            context->next_param = BENEFICIARY;
            break;

        case BENEFICIARY:  // to
            handle_beneficiary_function(msg, context);
            context->next_param = PATH;
            context->skip++;
            break;

        case PATH:  // len(path)
            context->next_param = TOKEN_SENT;
            break;

        case TOKEN_SENT:  // path[0]
            handle_token_sent_function(msg, context);
            context->next_param = TOKEN_RECEIVED;
            break;

        case TOKEN_RECEIVED:  // path[len(path) - 1]
            handle_token_received_function(msg, context);
            context->next_param = NONE;
            break; 
*/

        case NONE:
            break;

        default:
            PRINTF("Unsupported param\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}


/* Boilerplate example - just for reference
static void handle_swap_exact_eth_for_tokens_function(ethPluginProvideParameter_t *msg, context_t *context) {
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
*/

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
        PRINTF("Parameter Skipped, skipping next: %d parameters \n", context->skip);
    } else {

    // context->offset = 0;

    // adapt the cases and the names of the functions.

        switch (context->selectorIndex) {

            // *** Bep20
            case BEP20_APPROVE:
                handle_approve_function(msg, context);        
                break;

            // *** Venus vTokens and vBNB
            case VENUS_MINT_BNB:
                handle_no_param_function(msg, context);
                break;

            case VENUS_MINT:
            case VENUS_REDEEM:
            case VENUS_REDEEM_UNDERLYING:
            case VENUS_BORROW:
            case VENUS_REPAY_BORROW:
                handle_one_param_function(msg, context);
                break;

            case VENUS_REPAY_BORROW_BNB:
                handle_no_param_function(msg, context);
                break;

            case VENUS_REPAY_BORROW_ON_BEHALF:
                handle_repay_borrow_on_behalf_function(msg, context);
                break;
            
            //  *** Comptroller
            case VENUS_PROVIDE_COLLATERAL:
                handle_collateral_tokens_function(msg, context);
                break;

            case VENUS_REMOVE_COLLATERAL:
                handle_one_param_function(msg, context);
                break;

            // *** Vaults ***
            case VAULT_DEPOSIT:
            case VAULT_WITHDRAW_VAI:
                handle_one_param_function(msg, context);
                break;

            case VAULT_WITHDRAW_VRTXVS:
                handle_no_param_function(msg, context); //VRT has no parameters to handle
                break;

            case VAULT_DEPOSIT_TOKEN: 
            case VAULT_WITHDRAW_TOKEN_REQUEST:
            case VAULT_WITHDRAW_TOKEN_EXECUTE:
                handle_vault_token_function(msg, context);
                break;

            case VAULT_CLAIM:   // Cant get name of the Vault, removed from spec
                handle_no_param_function(msg, context);
                break;

            // *** Governance ***
            case VENUS_DELEGATE_VOTE:
                handle_one_param_function(msg, context);
                break;

            case VENUS_MAKE_PROPOSAL:
                handle_make_proposal_function(msg, context);
                break;   

            case VENUS_CAST_VOTE:
                handle_cast_vote_function(msg, context);
                break;

            case VENUS_VOTE_WITH_REASON:
                handle_vote_with_reason_function(msg, context);
                break;

            // *** Convert ***
            case VENUS_CONVERT_VRT:
                handle_one_param_function(msg, context);
                break;
    
            // *** Swap ***
            case SWAP_EXACT_TOKENS_FOR_TOKENS:
            case SWAP_EXACT_TOKENS_FOR_ETH:
                handle_swap_exact_tokens_function(msg, context);
                break;

            case SWAP_EXACT_ETH_FOR_TOKENS:
            case SWAP_ETH_FOR_EXACT_TOKENS:
            case SWAP_TOKENS_FOR_EXACT_TOKENS:
            case SWAP_TOKENS_FOR_EXACT_ETH:
                handle_swap_tokens_for_exact_tokens_function(msg, context);
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
        PRINTF("Parameter processed - Offset set: %d \n", context->offset);
        PRINTF("Parameter processed - Skipping next: %d parameters \n", context->skip);
    }
}