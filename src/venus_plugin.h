#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Name of the plugin.
#define PLUGIN_NAME "Venus"

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_VENUS_SELECTORS 30

// Common function to handle default network token
// Use `0xeeeee` as a dummy address to represent ETH.
extern const uint8_t VENUS_ETH_ADDRESS[ADDRESS_LENGTH];

// Adress 0x00000... used to indicate that the beneficiary is the sender.
extern const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH];

// Returns 1 if corresponding address is the address for the chain token (ETH, BNB, MATIC,
// etc.. are 0xeeeee...).
#define ADDRESS_IS_NETWORK_TOKEN(_addr) (!memcmp(_addr, VENUS_ETH_ADDRESS, ADDRESS_LENGTH))

/* using token1.found and token2.found rather than tokens.found
#define TOKEN_SENT_FOUND     1
#define TOKEN_RECEIVED_FOUND (1 << 1)
*/

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c, altered - array now in contract.c
typedef enum {
    BEP20_APPROVE = 0,                  // ERC20_APPROVE is supplied by default  TODO: check if we need this
    VENUS_MINT,
    VENUS_REDEEM_UNDERLYING,
    VENUS_REDEEM,
    VENUS_BORROW,
    VENUS_REPAY_BORROW,
    VENUS_REPAY_BORROW_ON_BEHALF,       // TODO: Check amount being repaid in provide_parameter
    VENUS_PROVIDE_COLLATERAL,           // TODO: Iterate through array of tokens in provide_parameter
    VENUS_REMOVE_COLLATERAL,
    VAULT_DEPOSIT,                      // TODO: Display name of vault
    VAULT_DEPOSIT_TOKEN,
    VAULT_WITHDRAW_VAI,
    VAULT_WITHDRAW_VRT,
    VAULT_WITHDRAW_TOKEN_REQUEST,
    VAULT_WITHDRAW_TOKEN_EXECUTE,
    VAULT_CLAIM,                        // TODO: Display name of vault
    VENUS_DELEGATE_VOTE,
    VENUS_MAKE_PROPOSAL,                // TODO: Extract proposal title from json array in provide_parameter
    VENUS_CAST_VOTE,
    VENUS_VOTE_WITH_REASON,
    VENUS_CONVERT_VRT,
    VENUS_WITHDRAW_VESTED_XVS,
    SWAP_EXACT_TOKENS_FOR_TOKENS,
    SWAP_EXACT_ETH_FOR_TOKENS,
    SWAP_EXACT_TOKENS_FOR_ETH,
    SWAP_TOKENS_FOR_EXACT_TOKENS,
    SWAP_ETH_FOR_EXACT_TOKENS,
    SWAP_TOKENS_FOR_EXACT_ETH,
    VENUS_MINT_VAI,
    VENUS_REPAY_VAI,
} venueSelector_t;

// Enumeration used to parse the smart contract data.
typedef enum {
    // *** Bep20
    APPROVE_SPENDER = 0,    // address of token for BEP20_APPROVE, followed by
    APPROVE_AMOUNT,         // uint256
    
    // *** Venus vToken   
    MINT_AMOUNT,            // uint256 single parameter for VENUS_MINT
    REDEEM_TOKENS,          // uint256 single parameter for VENUS_REDEEM
    REDEEM_AMOUNT,          // uint256 single parameter for VENUS_REDEEM_UNDERLYING
    BORROW_AMOUNT,          // uint256 single parameter for VENUS_BORROW
    REPAY_AMOUNT,           // uint256 single parameter for VENUS_REPAY_BORROW

    BORROWER,               // address for VENUS_REPAY_BORROW_ON_BEHALF_OF followed by
    VBNB_TOKEN,             // address
    
    // *** Comptroller
    COLLATERAL_TOKENS,      // address[] arrayof vTokens for VENUS_PROVIDE_COLLATERAL - TODO: Iterate through array in provid_parameter
    COLLATERAL_VTOKEN,      // address for VENUS_REMOVE_COLLATERAL

    //CLAIM_XVS_HOLDER address, CLAIM_XVS_vTOKENS array used for CLAIM_VENUS functions - Removed from Spec

    // *** Vaults
    DEPOSIT_AMOUNT,         // uint256 single parameter for VAULT_DEPOSIT

    REWARD_TOKEN,           // address for VAULT_DEPOSIT_TOKEN, VAULT_WITHDRAW_TOKEN_REQUEST and VAULT_WITHDRAW_EXECUTE
    VAULT_PID,              // uint256
    REWARD_TOKEN_AMOUNT,    // uint256 - not for VAULT_WITHDRAW_EXECUTE

    WITHDRAW_AMOUNT,        // uint256 single parameter for VAULT_WITHDRAW_VAI and VAULT_WITHDRAW_VRT

    VAULT_NAME,             // VAULT_CLAIM - has no parameters for VRT and VAI. For XVS claim call VAULT_DEPOSIT_TOKEN with zero 0 amount

    // *** Governance
    DELEGATEE,              // address single parameter for VENUS_DELEGATE_VOTE
    
    PROPOSAL_ID,            // uint256 for VENUS_CAST_VOTE and VENUS_VOTE_WITH_REASON
    SUPPORT,                // uint8 (corresponds to "For", "Against" and "Abstain")
    REASON,                 // not used with CAST_VOTE

    PROPOSAL_TITLE,         // "Show me the money" - dummy for now
    
    // TODO: default parameters are as follows
    /* VENUS_MAKE_PROSPOSAL extract "Title" from JSON stored in DESCRIPTION, normally the 2nd line
    PROPOSAL_TARGETS,       // address[] array for VENUS_MAKE_PROPOSAL
    PROPOSAL_VALUES,        // uint256[] array
    PROPOSAL_SIGNATURES,    // string[] array
    PROPOSAL_CALLDATA,      // bytes[] array
    PROPOSAL_DESCRIPTION,   // string
    PROPOSAL_TYPE,          // uint8
    */

    
    // *** Convert ***
    VRT_AMOUNT,              // uint256 single parameter VENUS_CONVERT_VRT
    CLAIM_XVS,               // VENUS_WITHDRAW_VESTED_XVS - has no parameters just displays "Claim XVS"

    // *** Swap *** Fields used in Swap
    AMOUNT_SENT,            // uint256 SWAP_TOKENS_FOR_EXACT, SWAP_EXACT_ETH_FOR_TOKENS
    AMOUNT_RECEIVED,        // uint256
    TOKEN_SENT,
    TOKEN_RECEIVED,
    PATH,                   // address[] array
    PATHS_OFFSET,            
    PATHS_LENGTH,             
    BENEFICIARY,            // address
    DEADLINE,               // uint256 not required by our plugin
    
    // *** VAI ***
    VAI_AMOUNT,             // uint256 single parameter for VENUS_MINT_VAI and VENUS_REPAY_VAI

    NONE,                   // we dont want one
    UNEXPECTED_PARAMETER,   // raise an error
} parameter;

// Same as the one initialized in `main.c`.
extern const uint32_t VENUS_SELECTORS[NUM_VENUS_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    // For display.
    uint8_t amount_sent[INT256_LENGTH];                 // 32
    uint8_t amount_received[INT256_LENGTH];             // 32
    uint8_t beneficiary[ADDRESS_LENGTH];                // 20
    uint8_t contract_address_sent[ADDRESS_LENGTH];      // 20
    uint8_t contract_address_received[ADDRESS_LENGTH];  // 20
    char ticker_sent[MAX_TICKER_LEN];                   // 12 bytes
    char ticker_received[MAX_TICKER_LEN];               // 12 bytes
    uint8_t decimals_sent;                              // 1 byte
    uint8_t decimals_received;                          // 1 byte

    //sub total 150 bytes - 10 bytes left

    /* Not enough room for more specific selectors, must map specifc fields to the one above as shown
    uint8_t dest[ADDRESS_LENGTH]; use beneficiary[ADDRESS_LENGTH]
    uint8_t collateral[ADDRESS_LENGTH]; use contract_address_sent[ADDRESS_LENGTH]
    uint8_t amount[INT256_LENGTH]; use amount_sent[INT256_LENGTH]
    uint8_t proposal_id[INT256_LENGTH]; use amount_sent[INT256_LENGTH]
    uint8_t support[INT256_LENGTH];   use amount_received[INT256_LENGTH]
    char ticker[MAX_TICKER_LEN]; ticker_sent 
    */

    uint8_t token1_found;                       // 1 byte
    uint8_t token2_found;                       // 1 byte

    uint8_t skip;                               // 1 byte
    uint8_t array_len;                          // 1 byte
    uint8_t checkpoint;                         // 1 byte

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.    - 1 byte
    uint16_t offset;     // Offset at which the array or struct starts.     - 2 bytes
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.                            - 1 byte

    // For both parsing and display.
    venusSelector_t selectorIndex;              // 1 byte

    // Sub total 10 bytes
    // Total 150 + 10 = 160 : 0 bytes left

} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);