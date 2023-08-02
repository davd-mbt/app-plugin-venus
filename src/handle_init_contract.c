#include "venus_plugin.h"

static int find_selector(uint32_t selector, const uint32_t *selectors, size_t n, venusSelector_t *out) {
    for (venusSelector_t i = 0; i < n; i++) {
        if (selector == selectors[i]) {
            *out = i;
            return 0;
        }
    }
    return -1;
}

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Venus content size is bigger than allowed: expected %d got %d \n",
                sizeof(context_t),
                msg->pluginContextLength);
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    uint32_t selector = U4BE(msg->selector, 0);
    if (find_selector(selector, VENUS_SELECTORS, NUM_VENUS_SELECTORS, &context->selectorIndex)) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {

        // *** Bep20
        case BEP20_APPROVE:
            context->next_param = APPROVE_SPENDER;
            break;

        // *** Venus vTokens and vBNB

        case VENUS_MINT_BNB:
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case VENUS_MINT:
            context->next_param = MINT_AMOUNT;
            break;

        case VENUS_REDEEM:
            context->next_param = REDEEM_TOKENS;
            break;

        case VENUS_REDEEM_UNDERLYING:
            context->next_param = REDEEM_AMOUNT;
            break;

        case VENUS_BORROW:
            context->next_param = BORROW_AMOUNT;
            break;

        case VENUS_REPAY_BORROW:
            context->next_param = REPAY_AMOUNT;
            break;

        case VENUS_REPAY_BORROW_BNB:
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case VENUS_REPAY_BORROW_ON_BEHALF:
            context->next_param = BORROWER;
            break;

        case VENUS_PROVIDE_COLLATERAL:
            context->next_param = COLLATERAL_TOKENS_OFFSET;
            break;

        case VENUS_REMOVE_COLLATERAL:
            context->next_param = COLLATERAL_VTOKEN;
            break;

        // *** Vaults ***
        case VAULT_DEPOSIT:
            context->next_param = DEPOSIT_AMOUNT;
            break;
        
        case VAULT_DEPOSIT_TOKEN:
        case VAULT_WITHDRAW_TOKEN_EXECUTE:
        case VAULT_WITHDRAW_TOKEN_REQUEST:
            context->next_param = REWARD_TOKEN;
            break;

        case VAULT_WITHDRAW_VAI:
            context->next_param = WITHDRAW_AMOUNT;
            break;

        case VAULT_WITHDRAW_VRTXVS:
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case VAULT_CLAIM:
            context->next_param = UNEXPECTED_PARAMETER;
            break;            

        // *** Governance ***

        case VENUS_DELEGATE_VOTE:
            context->next_param = DELEGATEE;
            break;

        case VENUS_MAKE_PROPOSAL:
            context->next_param = PROPOSAL_TARGETS_OFFSET;
            break;

        case VENUS_CAST_VOTE:
        case VENUS_VOTE_WITH_REASON:
            context->next_param = PROPOSAL_ID;
            break;

        case VENUS_CONVERT_VRT:
            context->next_param = VRT_AMOUNT;
            break;

        // *** Swap ***
        case SWAP_EXACT_TOKENS_FOR_TOKENS:
        case SWAP_EXACT_TOKENS_FOR_ETH:
            context->next_param = AMOUNT_SENT;
            break;

        case SWAP_TOKENS_FOR_EXACT_TOKENS:
        case SWAP_EXACT_ETH_FOR_TOKENS:
        case SWAP_ETH_FOR_EXACT_TOKENS:
        case SWAP_TOKENS_FOR_EXACT_ETH:
            context->next_param = AMOUNT_RECEIVED;
            break;

        // *** VAI ***
        case VENUS_MINT_VAI:
        case VENUS_REPAY_VAI:        
            context->next_param = VAI_AMOUNT;
            break;

        // Keep this
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
