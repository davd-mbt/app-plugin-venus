#include "venus_plugin.h"

// `msg->token1` and `msg->token2`. If those pointers are `NULL`, this means the ethereum app didn't
// find any info regarding the requested tokens!
void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    PRINTF("VENUS plugin provide token: 0x%p, 0x%p\n",
           msg->item1->token.address,
           msg->item2->token.address);

    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_sent)) {
        strlcpy(context->ticker_sent, DEFAULT_NETWORK_TICKER, sizeof(context->ticker_sent));
        context->decimals_sent = WEI_TO_ETHER;
        context->token1_found = true;
    } else if (msg->item1) {
        // The app found the information for the requested token!
        
        // Store its decimals.
        context->decimals_sent = msg->item1->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker_sent, (char *) msg->item1->token.ticker, sizeof(context->ticker_sent));
        // Keep track that we found the token.
        context->token1_found = true;
    } else {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token1_found = false;

        // If we wanted to add a screen, say a warning screen for example, we could instruct the
        // ethereum app to add an additional screen by setting `msg->additionalScreens` here, just
        // like so:
        // msg->additionalScreens = 1;
    }

    // Second token mostly relevant with SWAP
    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_received)) {
        strlcpy(context->ticker_received, DEFAULT_NETWORK_TICKER, sizeof(context->ticker_received));
        context->decimals_received = WEI_TO_ETHER;
        context->token2_found = true;        
    } else if (msg->item2) {
        // The Ethereum App found the information for the 2nd requested token!
        // Store its decimals.
        context->decimals_received = msg->item2->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker_received, (char *) msg->item2->token.ticker, sizeof(context->ticker_received));
        // Keep track that we found the token.
        context->token2_found = true;
    } else {
        // The Ethereum App did not manage to find the info for the 2nd requested token.
        context->token2_found = false;

        // If we wanted to add a screen, say a warning screen for example, we could instruct the
        // ethereum app to add an additional screen by setting `msg->additionalScreens` here, just
        // like so:
        // msg->additionalScreens = 1;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}