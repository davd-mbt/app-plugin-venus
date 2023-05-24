#include "venus_plugin.h"

// List of selectors supported by this plugin.

// *** Generic BEP20 token (USDT, USDC, BUSD etc.)

//Function: approve(address spender, uint256 amount)
//MethodID: 0x095ea7b3
static const uint32_t BEP20_APPROVE_SELECTOR = 0x095ea7b3;


//***   VENUS vTokens  and vBnb***

//Function: mint()
//MethodID: 0x1249c58b
static const uint32_t VENUS_MINT_BNB_SELECTOR = 0x1249c58b;

//Function: mint(uint256 mintAmount)
//MethodID: 0xa0712d68
static const uint32_t VENUS_MINT_SELECTOR = 0xa0712d68;

//Function: redeemUnderlying(uint256 redeemAmount)
//MethodID: 0x852a12e3
static const uint32_t VENUS_REDEEM_UNDERLYING_SELECTOR = 0x852a12e3;

//Function: redeem(uint256 redeemTokens)
//MethodID: 0xdb006a75
static const uint32_t VENUS_REDEEM_SELECTOR = 0xdb006a75;

//Function: borrow(uint256 borrowAmount)
//MethodID: 0xc5ebeaec
static const uint32_t VENUS_BORROW_SELECTOR = 0xc5ebeaec;

//Function: repayBorrow(uint256 repayAmount)
//MethodID: 0x0e752702
static const uint32_t VENUS_REPAY_BORROW_SELECTOR = 0x0e752702;

//Function: repayBorrow()
//MethodID: 0x4e4d9fea
static const uint32_t VENUS_REPAY_BORROW_BNB_SELECTOR = 0x4e4d9fea;

//*** VENUS Maximillion

//Function: repayBehalfExplicit(address borrower, address vBnb_)
//MethodID: 0x367b7f05
static const uint32_t VENUS_REPAY_BORROW_ON_BEHALF_SELECTOR = 0x367b7f05;

//***   VENUS Comptroller  ***

//Function: enterMarkets(address[] vTokens)
//MethodID: 0xc2998238
static const uint32_t VENUS_PROVIDE_COLLATERAL_SELECTOR = 0xc2998238;

//Function: exitMarket(address vTokenAddress)
//MethodID: 0xede4edd0
static const uint32_t VENUS_REMOVE_COLLATERAL_SELECTOR = 0xede4edd0;


// Claim Venus Functions On Hold at the request of Venus Team
//Function: claimVenus(address holder,address[] vTokens)
//MethodID: 0x86df31ee

//claimVenus(holder address)
//Methodid: 0xadcd5fb9

//claimVenusAsCollateral(holder address)
//Methodid: 0x7858524d


//***   VENUS VAULTS  ***

//VAI and VRT share the same MethodID for deposit function
//Function: deposit(uint256 _amount)
//MethodID: 0xb6b55f25
static const uint32_t VAULT_DEPOSIT_SELECTOR = 0xb6b55f25;

//XVS - If _amount is 0 then treat as a claim!
//Function: deposit(address _rewardToken, uint256 _pid, uint256 _amount)
//MethodID: 0x0efe6a8b
static const uint32_t VAULT_DEPOSIT_TOKEN_SELECTOR = 0x0efe6a8b;

//VAI
//Function: withdraw(uint256 _amount)
//MethodID: 0x2e1a7d4d
static const uint32_t VAULT_WITHDRAW_VAI_SELECTOR = 0x2e1a7d4d;

//VRT
//Function: withdraw()
//MethodID: 0x3ccfd60b
static const uint32_t VAULT_WITHDRAW_VRTXVS_SELECTOR = 0x3ccfd60b;

//Function: requestWithdrawal(address _rewardToken, uint256 _pid, uint256 _amount)
//MethodID: 0x115b512f
static const uint32_t VAULT_WITHDRAW_TOKEN_REQUEST_SELECTOR = 0x115b512f;

//Function: executeWithdrawal(address _rewardToken, uint256 _pid)
//MethodID: 0x7ac92456
static const uint32_t VAULT_WITHDRAW_TOKEN_EXECUTE_SELECTOR = 0x7ac92456;

//VAI and VRT share the same MethodID for claim function - XVS uses VAULT_DEPOSIT with a 0 amount
//Function: claim()
//MethodID: 0x4e71d92d
static const uint32_t VAULT_CLAIM_SELECTOR = 0x4e71d92d;

//***   VENUS GOVERNANCE  ***

//Function: delegate(address delegatee)
//MethodID: 0x5c19a95c
static const uint32_t VENUS_DELEGATE_VOTE_SELECTOR = 0x5c19a95c;

//Function: propose(address[] targets,uint256[] values,string[] signatures,bytes[] calldatas,string description,uint8 proposalType)
//MethodID: 0x164a1ab1
static const uint32_t VENUS_MAKE_PROPOSAL_SELECTOR = 0x164a1ab1;

//Function: castVote(uint256 proposalId,uint8 support)
//MethodID: 0x56781388
static const uint32_t VENUS_CAST_VOTE_SELECTOR = 0x56781388;

//Function: castVoteWithReason(uint256 proposalId,uint8 support,string reason)
//MethodID: 0x7b3c71d3
static const uint32_t VENUS_VOTE_WITH_REASON_SELECTOR = 0x7b3c71d3;


//Function: convert(uint256 vrtAmount)
//MethodID: 0xa3908e1b
static const uint32_t VENUS_CONVERT_VRT_SELECTOR = 0xa3908e1b;

//Function: withdraw()
//MethodID: 0x3ccfd60b
// duplicated above with VAULT_WITHDRAW_VRT_SELECTOR now changed to VAULT_WITHDRAW_VRTXVS_SELECTOR
//static const uint32_t VENUS_WITHDRAW_VESTED_XVS_SELECTOR = 0x3ccfd60b;

//***   VENUS SWAP  ***

//Function: swapExactTokensForTokens(uint256 amountIn,uint256 amountOutMin,address[] path,address to,uint256 deadline)
//MethodID: 0x38ed1739
static const uint32_t SWAP_EXACT_TOKENS_FOR_TOKENS_SELECTOR = 0x38ed1739;

//Function: swapExactETHForTokens(uint256 amountOutMin,address[] path,address to,uint256 deadline)
//MethodID: 0x7ff36ab5
static const uint32_t SWAP_EXACT_ETH_FOR_TOKENS_SELECTOR = 0x7ff36ab5;

//Function: swapExactTokensForETH(uint256 amountIn,uint256 amountOutMin,address[] path,address to,uint256 deadline)
//MethodID: 0x18cbafe5
static const uint32_t SWAP_EXACT_TOKENS_FOR_ETH_SELECTOR = 0x18cbafe5;

//Function: swapTokensForExactTokens(uint256 amountOut,uint256 amountInMax,address[] path,address to,uint256 deadline)
//MethodID: 0x8803dbee
static const uint32_t SWAP_TOKENS_FOR_EXACT_TOKENS_SELECTOR = 0x8803dbee;

//Function: swapETHForExactTokens(uint256 amountOut,address[] path,address to,uint256 deadline)
//MethodID: 0xfb3bdb41
static const uint32_t SWAP_ETH_FOR_EXACT_TOKENS_SELECTOR = 0xfb3bdb41;

//Function: swapTokensForExactETH(uint256 amountOut,uint256 amountInMax,address[] path,address to,uint256 deadline)
//MethodID: 0x4a25d94a
static const uint32_t SWAP_TOKENS_FOR_EXACT_ETH_SELECTOR = 0x4a25d94a;

//***   VENUS VAI  ***

//Function: mintVAI(uint256 mintVAIAmount)
//MethodID: 0x4712ee7d
static const uint32_t VENUS_MINT_VAI_SELECTOR = 0x4712ee7d;

//Function: repayVAI(uint256 repayVAIAmount)
//MethodID: 0x6fe74a21
static const uint32_t VENUS_REPAY_VAI_SELECTOR = 0x6fe74a21;

// Array of all the different boilerplate selectors. Make sure this follows the same order as the
// enum defined in `venus_plugin.h`
// Use the names of the array declared above.
const uint32_t VENUS_SELECTORS[NUM_VENUS_SELECTORS] = {
    BEP20_APPROVE_SELECTOR,
    VENUS_MINT_BNB_SELECTOR,
    VENUS_MINT_SELECTOR,
    VENUS_REDEEM_UNDERLYING_SELECTOR,
    VENUS_REDEEM_SELECTOR,
    VENUS_BORROW_SELECTOR,
    VENUS_REPAY_BORROW_SELECTOR,
    VENUS_REPAY_BORROW_BNB_SELECTOR,
    VENUS_REPAY_BORROW_ON_BEHALF_SELECTOR,
    VENUS_PROVIDE_COLLATERAL_SELECTOR,
    VENUS_REMOVE_COLLATERAL_SELECTOR,
    VAULT_DEPOSIT_SELECTOR,
    VAULT_DEPOSIT_TOKEN_SELECTOR,
    VAULT_WITHDRAW_VAI_SELECTOR,
    VAULT_WITHDRAW_VRTXVS_SELECTOR,
    VAULT_WITHDRAW_TOKEN_REQUEST_SELECTOR,
    VAULT_WITHDRAW_TOKEN_EXECUTE_SELECTOR,
    VAULT_CLAIM_SELECTOR,
    VENUS_DELEGATE_VOTE_SELECTOR,
    VENUS_MAKE_PROPOSAL_SELECTOR,
    VENUS_CAST_VOTE_SELECTOR,
    VENUS_VOTE_WITH_REASON_SELECTOR,
    VENUS_CONVERT_VRT_SELECTOR,
    SWAP_EXACT_TOKENS_FOR_TOKENS_SELECTOR,
    SWAP_EXACT_ETH_FOR_TOKENS_SELECTOR,
    SWAP_EXACT_TOKENS_FOR_ETH_SELECTOR,
    SWAP_TOKENS_FOR_EXACT_TOKENS_SELECTOR,
    SWAP_ETH_FOR_EXACT_TOKENS_SELECTOR,
    SWAP_TOKENS_FOR_EXACT_ETH_SELECTOR,
    VENUS_MINT_VAI_SELECTOR,
    VENUS_REPAY_VAI_SELECTOR,
};

// using `0xeeeee` as a dummy address to represent ETH/BNB
const uint8_t VENUS_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00, 0x10, 0x10};

// Used to indicate that the beneficiary should be the sender.
const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};