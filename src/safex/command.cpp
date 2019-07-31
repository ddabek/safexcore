//
// Created by amarko on 4.3.19..
//

#include "cryptonote_config.h"
#include "command.h"
#include "cryptonote_core/cryptonote_tx_utils.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "cryptonote_config.h"
#include "cryptonote_core/blockchain.h"


#include "fee_distribution.h"


#undef SAFEX_DEFAULT_LOG_CATEGORY
#define SAFEX_DEFAULT_LOG_CATEGORY "safex"

namespace safex
{


  token_stake_result* token_stake::execute(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {

    //per input execution, one input could be less than SAFEX_MINIMUM_TOKEN_STAKE_AMOUNT, all inputs must be SAFEX_MINIMUM_TOKEN_STAKE_AMOUNT
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((tools::is_whole_token_amount(this->get_staked_token_amount())), "Staked input is not whole token amount", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == this->get_staked_token_amount()), "Input amount differs from token stake command amount", this->get_command_type());

    token_stake_result *cr = new token_stake_result{};
    cr->token_amount = txin.token_amount;
    cr->block_number = blokchainDB.height();
    cr->valid = true;
    cr->status = execution_status::ok;

    MINFO("Block height:" << cr->block_number << " interval:" << calculate_interval_for_height(blokchainDB.height(), blokchainDB.get_net_type()) << " stake tokens:" << cr->token_amount);

    return cr;
  }

  execution_status token_stake::validate(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {
    execution_status result = execution_status::ok;

    //per input execution, one input could be less than SAFEX_MINIMUM_TOKEN_STAKE_AMOUNT, all inputs must be SAFEX_MINIMUM_TOKEN_STAKE_AMOUNT
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((tools::is_whole_token_amount(this->get_staked_token_amount())), "Staked input is not whole token amount", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == this->get_staked_token_amount()), "Input amount differs from token stake command amount", this->get_command_type());

    return result;
  }

  token_unstake_result* token_unstake::execute(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets.size() == 1), "Only one locked token output could be processed per input", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets[0] == this->get_staked_token_output_index()), "Locked token output ID does not match", this->get_command_type());

    //todo Get data about locked token output from database using its index
    //todo check if db output amount is same as txin amount
    //todo check if minimum amount of time is fulfilled


    token_unstake_result *cr = new token_unstake_result{};
    cr->token_amount = txin.token_amount;
    cr->block_number = blokchainDB.height();

    uint64_t locked_token_output_index = txin.key_offsets[0];
    cr->interest = calculate_token_interest(locked_token_output_index, cr->block_number, cr->token_amount);
    cr->valid = true;
    cr->status = execution_status::ok;

    MINFO("Block height:" << cr->block_number << " interval:" << calculate_interval_for_height(blokchainDB.height(), blokchainDB.get_net_type()) << " unstake tokens:" << cr->token_amount);

    return cr;
  }

  execution_status token_unstake::validate(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {

    execution_status result = execution_status::ok;

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets.size() == 1), "Only one locked token output could be processed per input", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets[0] == this->get_staked_token_output_index()), "Locked token output ID does not match", this->get_command_type());

    //todo Get data about locked token output from database using its index
    //todo check if db output amount is same as txin amount
    //todo check if minimum amount of time is fulfilled

    return result;
  }


  token_collect_result* token_collect::execute(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets.size() == 1), "Only one locked token output could be processed per input", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets[0] == this->get_staked_token_output_index()), "Locked token output ID does not match", this->get_command_type());

    //todo Get data about locked token output from database using its index
    //todo check if db output amount is same as txin amount
    //todo check if minimum amount of time is fulfilled


    token_collect_result *cr = new token_collect_result{};
    cr->token_amount = txin.token_amount;
    cr->block_number = blokchainDB.height();

    uint64_t locked_token_output_index = txin.key_offsets[0];
    cr->interest = calculate_token_interest(locked_token_output_index, cr->block_number, cr->token_amount);
    cr->valid = true;
    cr->status = execution_status::ok;

    return cr;
  }

  execution_status token_collect::validate(const cryptonote::BlockchainDB &blokchainDB, const cryptonote::txin_to_script &txin)
  {

    execution_status result = execution_status::ok;

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets.size() == 1), "Only one locked token output could be processed per input", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.key_offsets[0] == this->get_staked_token_output_index()), "Locked token output ID does not match", this->get_command_type());

    //todo Get data about locked token output from database using its index
    //todo check if db output amount is same as txin amount
    //todo check if minimum amount of time is fulfilled

    return result;
  }


  donate_fee_result* donate_fee::execute(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Amount to donate must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Tokens could not be donated to network ", this->get_command_type());

    donate_fee_result *cr = new donate_fee_result{};
    cr->amount = txin.amount;
    cr->valid = true;
    cr->status = execution_status::ok;

    return cr;
  };

  execution_status donate_fee::validate(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {

    execution_status result = execution_status::ok;

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Amount to donate must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Tokens could not be donated to network ", this->get_command_type());

    return result;
  };

  simple_purchase_result* simple_purchase::execute(const cryptonote::BlockchainDB &blockchain, const cryptonote::txin_to_script &txin) {
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Purchase amount must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Could not purchase with tokens ", this->get_command_type());

    simple_purchase_result *cr = new simple_purchase_result{};
    cr->network_fee = calculate_safex_network_fee(txin.amount, blockchain.get_net_type(), txin.command_type);
    cr->cash_amount = txin.amount - cr->network_fee;

    cr->valid = true;
    cr->status = execution_status::ok;

    return cr;
  };

  execution_status simple_purchase::validate(const cryptonote::BlockchainDB &blockchain, const cryptonote::txin_to_script &txin) {

    execution_status result = execution_status::ok;

    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Purchase amount must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Could not purchase with tokens ", this->get_command_type());

    return result;
  };


  distribute_fee_result* distribute_fee::execute(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Amount to donate must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Tokens could not be donated to network ", this->get_command_type());

    distribute_fee_result *cr = new distribute_fee_result{};
    cr->amount = txin.amount;
    cr->valid = true;
    cr->status = execution_status::ok;
    return cr;
  };

  execution_status distribute_fee::validate(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {

    execution_status result = execution_status::ok;
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.amount > 0), "Amount to donate must be greater than zero ", this->get_command_type());
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount == 0), "Tokens could not be donated to network ", this->get_command_type());

    return result;
  };

  create_account_result* create_account::execute(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount >= SAFEX_CREATE_ACCOUNT_TOKEN_LOCK_FEE), "Create account requires minimum "+
      std::to_string(SAFEX_CREATE_ACCOUNT_TOKEN_LOCK_FEE)+" tokens", this->get_command_type());



    //todo chek if account username is valid
    //todo check if account username already exists
    //todo check account description size



    create_account_result *cr = new create_account_result{this->username, this->pkey, this->account_data};
    cr->valid = true;
    cr->status = execution_status::ok;

    return cr;
  };

  execution_status create_account::validate(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    SAFEX_COMMAND_CHECK_AND_ASSERT_THROW_MES((txin.token_amount >= SAFEX_CREATE_ACCOUNT_TOKEN_LOCK_FEE), "Create account requires minimum "+
                  std::to_string(SAFEX_CREATE_ACCOUNT_TOKEN_LOCK_FEE)+" tokens", this->get_command_type());

    execution_status result = execution_status::ok;
    //todo chek if account username is valid
    //todo check if account username already exists
    //todo check account description size



    return result;
  };

  edit_account_result* edit_account::execute(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    edit_account_result *cr = new edit_account_result{};

    //todo check if account username is valid and exists
    //todo check account signature for new data


    cr->valid = true;
    cr->status = execution_status::ok;
    return cr;
  };

  execution_status edit_account::validate(const cryptonote::BlockchainDB &blokchain, const cryptonote::txin_to_script &txin) {
    execution_status result = execution_status::ok;

    //todo check if account username is valid and exists
    //todo check account signature for new data

    return result;
  };


  bool validate_safex_command(const cryptonote::BlockchainDB &blockchain, const cryptonote::txin_to_script &txin)
  {
    //parse command and execute it
    try
    {
      std::unique_ptr<command> cmd = safex_command_serializer::parse_safex_object(txin.script, txin.command_type);
      std::shared_ptr<execution_result> result{cmd->execute(blockchain, txin)};
      if (result->status != execution_status::ok)
      {
        LOG_ERROR("Execution of safex command failed, status:" << static_cast<int>(result->status));
        return false;
      }
    }
    catch (command_exception &ex)
    {
      LOG_ERROR("Error in safex command execution:" << ex.what());
      return false;
    }


    return true;
  }


  bool execute_safex_command(const cryptonote::BlockchainDB &blockchain, const cryptonote::txin_to_script &txin)
  {
    //parse command and execute it
    try
    {
      std::unique_ptr<command> cmd = safex_command_serializer::parse_safex_object(txin.script, txin.command_type);
      std::shared_ptr<execution_result> result{cmd->execute(blockchain, txin)};
      if (result->status != execution_status::ok)
      {
        LOG_ERROR("Execution of safex command failed, status:" << static_cast<int>(result->status));
        return false;
      }
    }
    catch (command_exception &ex)
    {
      LOG_ERROR("Error in safex command execution:" << ex.what());
      return false;
    }


    return true;
  }


}