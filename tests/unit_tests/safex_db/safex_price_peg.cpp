// Copyright (c) 2018, The Safex Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2014-2018 The Monero Project

#include <boost/filesystem.hpp>
#include <cstdio>
#include <iostream>
#include <chrono>

#include "gtest/gtest.h"

#include "string_tools.h"
#include "blockchain_db/blockchain_db.h"
#include "blockchain_db/lmdb/db_lmdb.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "safex/safex_account.h"
#include "safex/safex_price_peg.h"
#include "../safex_test_common.h"


using namespace cryptonote;
using epee::string_tools::pod_to_hex;

#define ASSERT_HASH_EQ(a, b) ASSERT_EQ(pod_to_hex(a), pod_to_hex(b))

namespace
{  // anonymous namespace

  const int NUMBER_OF_BLOCKS = 30;
  const int NUMBER_OF_BLOCKS1 = 15;
  const int NUMBER_OF_BLOCKS2 = 20;
  const int NUMBER_OF_BLOCKS3 = 30;
  const uint64_t default_miner_fee = ((uint64_t) 500000000);
  const std::string bitcoin_tx_hashes_str[6] = {"3b7ac2a66eded32dcdc61f0fec7e9ddb30ccb3c6f5f06c0743c786e979130c5f", "3c904e67190d2d8c5cc93147c1a3ead133c61fc3fa578915e9bf95544705e63c",
                                                "2d825e690c4cb904556285b74a6ce565f16ba9d2f09784a7e5be5f7cdb05ae1d", "89352ec1749c872146eabddd56cd0d1492a3be6d2f9df98f6fbbc0d560120182"};

  class SafexCreatePricePegCommand : public ::testing::Test
  {
   public:
      SafexCreatePricePegCommand() {
        crypto::public_key pubKey;
        epee::string_tools::hex_to_pod("229d8c9229ba7aaadcd575cc825ac2bd0301fff46cc05bd01110535ce43a15d1", pubKey);
        keys.push_back(pubKey);
     }
   protected:
     std::vector<crypto::public_key> keys;
     TestDB m_db;
 };

  class SafexUpdatePricePegCommand : public ::testing::Test
  {
   public:
      SafexUpdatePricePegCommand() {
        crypto::public_key pubKey;
        epee::string_tools::hex_to_pod("229d8c9229ba7aaadcd575cc825ac2bd0301fff46cc05bd01110535ce43a15d1", pubKey);
        keys.push_back(pubKey);
     }
   protected:
     std::vector<crypto::public_key> keys;
     TestDB m_db;
 };

  template<typename T>
  class SafexPricePegTest : public testing::Test
  {
    protected:
      SafexPricePegTest() : m_db(new T(false, cryptonote::network_type::FAKECHAIN)), m_hardfork(*m_db, 1, 0)
      {
        m_test_sizes = std::vector<size_t>(NUMBER_OF_BLOCKS, 0);
        m_test_coins = std::vector<uint64_t>(NUMBER_OF_BLOCKS, 60);
        m_test_coins[0] = 2000 * SAFEX_CASH_COIN; //genesis tx airdrop
        m_test_tokens = std::vector<uint64_t>(NUMBER_OF_BLOCKS, 0);
        m_test_tokens[0] = 4000 * SAFEX_TOKEN;
        m_test_diffs = std::vector<difficulty_type>(NUMBER_OF_BLOCKS, 200);
        m_test_diffs[0] = 1;
        m_test_diffs[1] = 100;
        m_test_diffs[2] = 180;

        m_miner_acc.generate();
        m_users_acc[0].generate();
        m_users_acc[1].generate();

        m_safex_account1_keys.generate();
        m_safex_account2_keys.generate();



        m_safex_account1.username = "user1";
        m_safex_account1.pkey = m_safex_account1_keys.get_keys().m_public_key;
        m_safex_account1.account_data = {'s','m','o','r'};
        m_safex_account2.username = "user2";
        m_safex_account2.pkey = m_safex_account2_keys.get_keys().m_public_key;

        std::cout << "Alice public key: " << epee::string_tools::pod_to_hex(m_safex_account1_keys.get_keys().m_public_key) << std::endl;
        std::cout << "Alice private key: " << epee::string_tools::pod_to_hex(m_safex_account1_keys.get_keys().m_secret_key) << std::endl;

        const std::string data1_new_str = "Another data tesst for edit";
        data1_new = std::vector<uint8_t>(data1_new_str.begin(), data1_new_str.end());

       m_safex_price_pegs.emplace_back("USD price peg",m_safex_account1.username, "USD", "xcalibra USD price peg", 30);
       m_safex_price_pegs.emplace_back("RSD price peg",m_safex_account2.username, "RSD", "xcalibra RSD price peg", 100);

       m_edited_safex_price_peg = m_safex_price_pegs[1];

       m_edited_safex_price_peg.rate = 40;

       for (int i = 0; i < NUMBER_OF_BLOCKS; i++)
        {
          block blk;
          std::list<cryptonote::transaction> tx_list; // fill tx list with transactions for that block
          crypto::hash prev_hash = boost::value_initialized<crypto::hash>();/* null hash*/

          if (i > 0) prev_hash = cryptonote::get_block_hash(m_blocks[i - 1]);

          if (i == 0)
          {
            //skip, genesis block
          }
          else if (i == 1)
          {
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_migration_tx_to_key(m_txmap, m_blocks, tx, m_miner_acc, m_users_acc[0], m_test_tokens[0], default_miner_fee, get_hash_from_string(bitcoin_tx_hashes_str[0]));
            m_txmap[get_transaction_hash(tx)] = tx;
          }
          else if (i == 2)
          {
            //distribute tokens and coins to accounts as starter
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_token_tx_to_key(m_txmap, m_blocks, tx, m_users_acc[0], m_users_acc[1],  1000 * SAFEX_TOKEN, default_miner_fee, 0);
            m_txmap[get_transaction_hash(tx)] = tx;

            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx2 = tx_list.back();                                                           \
            construct_tx_to_key(m_txmap, m_blocks, tx2, m_miner_acc, m_users_acc[0], 100 * SAFEX_CASH_COIN, default_miner_fee, 0);
            m_txmap[get_transaction_hash(tx2)] = tx2;

            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx3 = tx_list.back();                                                           \
            construct_tx_to_key(m_txmap, m_blocks, tx3, m_miner_acc, m_users_acc[1], 200 * SAFEX_CASH_COIN, default_miner_fee, 0);
            m_txmap[get_transaction_hash(tx3)] = tx3;
          }
          else if (i == 5)
          {
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_create_account_transaction(m_txmap, m_blocks, tx, m_users_acc[0], default_miner_fee, 0, m_safex_account1.username, m_safex_account1.pkey, m_safex_account1.account_data, m_safex_account1_keys.get_keys());
            m_txmap[get_transaction_hash(tx)] = tx;

            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx2 = tx_list.back();                                                           \
            construct_create_account_transaction(m_txmap, m_blocks, tx2, m_users_acc[1], default_miner_fee, 0, m_safex_account2.username, m_safex_account2.pkey, m_safex_account2.account_data, m_safex_account2_keys.get_keys());
            m_txmap[get_transaction_hash(tx2)] = tx2;
          }
          else if (i == 7)
          {
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_create_price_peg_transaction(m_txmap, m_blocks, tx, m_users_acc[0], default_miner_fee, 0, m_safex_account1.pkey, m_safex_price_pegs[0], m_safex_account1_keys.get_keys());
            m_txmap[get_transaction_hash(tx)] = tx;
          }
          else if (i == 8)
          {
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_create_price_peg_transaction(m_txmap, m_blocks, tx, m_users_acc[1], default_miner_fee, 0, m_safex_account2.pkey, m_safex_price_pegs[1], m_safex_account2_keys.get_keys());
            m_txmap[get_transaction_hash(tx)] = tx;
          }
          else if (i == 15)
          {
            tx_list.resize(tx_list.size() + 1);
            cryptonote::transaction &tx = tx_list.back();                                                           \
            construct_update_price_peg_transaction(m_txmap, m_blocks, tx, m_users_acc[1], default_miner_fee, 0, m_safex_account2.pkey, m_edited_safex_price_peg, m_safex_account2_keys.get_keys());
            m_txmap[get_transaction_hash(tx)] = tx;
          }

          construct_block(blk, i, prev_hash, m_miner_acc, 0, m_test_sizes[i], tx_list);

          m_txs.push_back(std::vector<cryptonote::transaction>{tx_list.begin(), tx_list.end()});
          m_blocks.push_back(blk);
        }
      }


      ~SafexPricePegTest()
      {
        delete m_db;
        remove_files(m_filenames, m_prefix);
      }

      BlockchainDB *m_db;
      HardFork m_hardfork;
      std::string m_prefix;
      std::vector<block> m_blocks;
      //std::unordered_map<crypto::hash, cryptonote::transaction>
      map_hash2tx_t m_txmap; //vector of all transactions
      std::vector<std::vector<transaction> > m_txs;
      std::vector<std::string> m_filenames;

      cryptonote::account_base m_miner_acc;
      cryptonote::account_base m_users_acc[2];

      std::vector<size_t> m_test_sizes;
      std::vector<uint64_t> m_test_coins;
      std::vector<uint64_t> m_test_tokens;
      std::vector<difficulty_type> m_test_diffs;

      safex::safex_account_key_handler m_safex_account1_keys{};
      safex::safex_account_key_handler m_safex_account2_keys{};
      safex::safex_account m_safex_account1;
      safex::safex_account m_safex_account2;

      std::vector<safex::safex_price_peg> m_safex_price_pegs;
      safex::safex_price_peg m_edited_safex_price_peg;

      std::vector<uint8_t> data1_new;


      void init_hard_fork()
      {
        m_hardfork.init();
        m_db->set_hard_fork(&m_hardfork);
      }

      void get_filenames()
      {
        m_filenames = m_db->get_filenames();
        for (auto &f : m_filenames)
        {
          std::cerr << "File created by test: " << f << std::endl;
        }
      }


      void set_prefix(const std::string &prefix)
      {
        m_prefix = prefix;
      }
  };

  using testing::Types;

  typedef Types<BlockchainLMDB> implementations;

  TYPED_TEST_CASE(SafexPricePegTest, implementations);

#if 1

  TEST_F(SafexCreatePricePegCommand, HandlesCorruptedArrayOfBytes)
  {

    std::vector<uint8_t> serialized_command = {0x32, 0x32, 0x13, 0x43, 0x12, 0x3, 0x4, 0x5, 0x5, 0x6, 0x32, 0x12, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};

    //deserialize
    EXPECT_THROW(safex::safex_command_serializer::parse_safex_object(serialized_command, safex::command_t::create_price_peg), safex::command_exception);

  }

  TEST_F(SafexUpdatePricePegCommand, HandlesCorruptedArrayOfBytes)
  {

    std::vector<uint8_t> serialized_command = {0x32, 0x32, 0x13, 0x43, 0x12, 0x3, 0x4, 0x5, 0x5, 0x6, 0x32, 0x12, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};

    //deserialize
    EXPECT_THROW(safex::safex_command_serializer::parse_safex_object(serialized_command, safex::command_t::update_price_peg), safex::command_exception);

  }

  TEST_F(SafexCreatePricePegCommand, HandlesUnknownProtocolVersion)
  {

    safex::create_price_peg_data price_peg_data{};
    try
    {
      safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION + 1, price_peg_data};
      FAIL() << "Should throw exception with message invalid command";
    }
    catch (safex::command_exception &exception)
    {
      ASSERT_STREQ(std::string(("Unsupported command protocol version " + std::to_string(SAFEX_COMMAND_PROTOCOL_VERSION + 1))).c_str(), std::string(exception.what()).c_str());
    }
    catch (...)
    {
      FAIL() << "Unexpected exception";
    }
  }

  TEST_F(SafexUpdatePricePegCommand, HandlesUnknownProtocolVersion)
  {

    safex::update_price_peg_data price_peg_data{};
    try
    {
      safex::update_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION + 1, price_peg_data};
      FAIL() << "Should throw exception with message invalid command";
    }
    catch (safex::command_exception &exception)
    {
      ASSERT_STREQ(std::string(("Unsupported command protocol version " + std::to_string(SAFEX_COMMAND_PROTOCOL_VERSION + 1))).c_str(), std::string(exception.what()).c_str());
    }
    catch (...)
    {
      FAIL() << "Unexpected exception";
    }
  }

  TEST_F(SafexCreatePricePegCommand, HandlesCommandParsing)
  {

     cryptonote::account_base acc_base;
     acc_base.generate();

     safex::safex_account_key_handler m_safex_account_keys;
     m_safex_account_keys.generate();

     safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", "username","USD","Some description", 1828);

    safex::create_price_peg_data price_peg_data{sfx_price_peg};

    safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};
    //serialize
    std::vector<uint8_t> serialized_command;
    safex::safex_command_serializer::serialize_safex_object(command1, serialized_command);

    safex::command_t command_type = safex::safex_command_serializer::get_command_type(serialized_command);
    ASSERT_EQ(command_type, safex::command_t::create_price_peg) << "Safex create price peg command type not properly parsed from binary blob";

    //deserialize
    std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(serialized_command, safex::command_t::create_price_peg);

    ASSERT_EQ(command1.get_version(), command2->get_version()) << "Original and deserialized command must have same version";
    ASSERT_EQ(command1.get_command_type(), command2->get_command_type()) << "Original and deserialized command must have same command type";
    ASSERT_EQ(command1.get_price_peg_id(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_price_peg_id()) << "Original and deserialized command must have same price peg ID";
    ASSERT_EQ(command1.get_creator(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_creator()) << "Original and deserialized command must have same creator";
    ASSERT_EQ(command1.get_title(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_title()) << "Original and deserialized command must have same title";
    ASSERT_EQ(command1.get_description(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_description()) << "Original and deserialized command must have same description";
    ASSERT_EQ(command1.get_currency(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_currency()) << "Original and deserialized command must have same currency";
    ASSERT_EQ(command1.get_rate(), dynamic_cast<safex::create_price_peg*>(command2.get())->get_rate()) << "Original and deserialized command must have same rate";

  }

  TEST_F(SafexUpdatePricePegCommand, HandlesCommandParsing)
  {

     cryptonote::account_base acc_base;
     acc_base.generate();

     safex::safex_account_key_handler m_safex_account_keys;
     m_safex_account_keys.generate();

     safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", "username","USD","Some description", 1828);

     safex::update_price_peg_data price_peg_data{sfx_price_peg};

     safex::update_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};
     //serialize
     std::vector<uint8_t> serialized_command;
     safex::safex_command_serializer::serialize_safex_object(command1, serialized_command);

     safex::command_t command_type = safex::safex_command_serializer::get_command_type(serialized_command);
     ASSERT_EQ(command_type, safex::command_t::update_price_peg) << "Safex create price peg command type not properly parsed from binary blob";

     //deserialize
     std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(serialized_command, safex::command_t::update_price_peg);

     ASSERT_EQ(command1.get_version(), command2->get_version()) << "Original and deserialized command must have same version";
     ASSERT_EQ(command1.get_command_type(), command2->get_command_type()) << "Original and deserialized command must have same command type";
     ASSERT_EQ(command1.get_price_peg_id(), dynamic_cast<safex::update_price_peg*>(command2.get())->get_price_peg_id()) << "Original and deserialized command must have same price peg ID";
     ASSERT_EQ(command1.get_rate(), dynamic_cast<safex::update_price_peg*>(command2.get())->get_rate()) << "Original and deserialized command must have same rate";

  }

  TYPED_TEST(SafexPricePegTest, CreatePricePegCommand) {
        boost::filesystem::path tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
        std::string dirPath = tempPath.string();

        bool result;

        this->set_prefix(dirPath);

        // make sure open does not throw
        ASSERT_NO_THROW(this->m_db->open(dirPath));
        this->get_filenames();
        this->init_hard_fork();

        for (int i = 0; i < NUMBER_OF_BLOCKS1; i++) {
            ASSERT_NO_THROW(this->m_db->add_block(this->m_blocks[i], this->m_test_sizes[i], this->m_test_diffs[i],
                                                  this->m_test_coins[i], this->m_test_tokens[i], this->m_txs[i]));
        }
        //Checking created price pegs
        for (auto safex_price_peg: this->m_safex_price_pegs) {

            std::vector<safex::safex_price_peg> saved_price_pegs;
            result = this->m_db->get_safex_price_pegs(saved_price_pegs,safex_price_peg.currency);
            ASSERT_TRUE(result);

            safex::safex_price_peg saved_price_peg = saved_price_pegs[0];

            ASSERT_TRUE(std::equal(safex_price_peg.description.begin(), safex_price_peg.description.end(),
                                   saved_price_peg.description.begin()));
            ASSERT_EQ(safex_price_peg.title,saved_price_peg.title);
            ASSERT_EQ(safex_price_peg.currency,saved_price_peg.currency);
            ASSERT_EQ(safex_price_peg.creator,saved_price_peg.creator);
            ASSERT_EQ(safex_price_peg.rate,saved_price_peg.rate);
            ASSERT_EQ(safex_price_peg.price_peg_id,saved_price_peg.price_peg_id);

        }

        for (int i = NUMBER_OF_BLOCKS1; i < NUMBER_OF_BLOCKS2; i++) {
            ASSERT_NO_THROW(this->m_db->add_block(this->m_blocks[i], this->m_test_sizes[i], this->m_test_diffs[i],
                                                  this->m_test_coins[i], this->m_test_tokens[i], this->m_txs[i]));
        }

        //Checking edited price peg

        safex::safex_price_peg saved_price_peg;
        result = this->m_db->get_safex_price_peg(this->m_edited_safex_price_peg.price_peg_id,saved_price_peg);
        ASSERT_TRUE(result);

        ASSERT_TRUE(std::equal(this->m_edited_safex_price_peg.description.begin(), this->m_edited_safex_price_peg.description.end(),
                               saved_price_peg.description.begin()));
        ASSERT_EQ(this->m_edited_safex_price_peg.title,saved_price_peg.title);
        ASSERT_EQ(this->m_edited_safex_price_peg.currency,saved_price_peg.currency);
        ASSERT_EQ(this->m_edited_safex_price_peg.creator,saved_price_peg.creator);
        ASSERT_EQ(this->m_edited_safex_price_peg.rate,saved_price_peg.rate);
        ASSERT_EQ(this->m_edited_safex_price_peg.price_peg_id,saved_price_peg.price_peg_id);



        for (int i = NUMBER_OF_BLOCKS2; i < NUMBER_OF_BLOCKS3; i++) {
            ASSERT_NO_THROW(this->m_db->add_block(this->m_blocks[i], this->m_test_sizes[i], this->m_test_diffs[i],
                                                  this->m_test_coins[i], this->m_test_tokens[i], this->m_txs[i]));
        }


    ASSERT_NO_THROW(this->m_db->close());

  }

  TYPED_TEST(SafexPricePegTest, CreatePricePegExceptions) {
        boost::filesystem::path tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
        std::string dirPath = tempPath.string();

        this->set_prefix(dirPath);

        // make sure open does not throw
        ASSERT_NO_THROW(this->m_db->open(dirPath));
        this->get_filenames();
        this->init_hard_fork();

        for (int i = 0; i < NUMBER_OF_BLOCKS1; i++) {
            ASSERT_NO_THROW(this->m_db->add_block(this->m_blocks[i], this->m_test_sizes[i], this->m_test_diffs[i],
                                                  this->m_test_coins[i], this->m_test_tokens[i], this->m_txs[i]));
        }

        // Safex account doesn't exist
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);
          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", "username","USD","Some description", 1828);

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_account_non_existant);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex account doesn't exist";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg ID already exists
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::create_price_peg_data price_peg_data{this->m_safex_price_pegs[0]};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_already_exists);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg ID already in the DB";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg name too big
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"USD","Some description", 1828);

          sfx_price_peg.title = "";
          for (int i=0; i < SAFEX_PRICE_PEG_NAME_MAX_SIZE + 2 ; i++) {
              sfx_price_peg.title += "x";
          }

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_data_too_big);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg title too big";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg curreny name too big
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"USD","Some description", 1828);

          sfx_price_peg.currency = "";
          for (int i=0; i < SAFEX_PRICE_PEG_CURRENCY_MAX_SIZE + 2 ; i++) {
              sfx_price_peg.currency += "x";
          }

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_data_too_big);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg currency name too big";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg description too big
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"USD","Some description", 1828);

          sfx_price_peg.description.clear();
          for (int i=0; i < SAFEX_PRICE_PEG_DATA_MAX_SIZE + 2 ; i++) {
              sfx_price_peg.description.push_back('x');
          }

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_data_too_big);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg currency description too big";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg curreny name invalid
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"US-D","Some description", 1828);

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_bad_currency_format);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg currency name invalid";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg rate zero
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::create_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"USD","Some description", 1828);

          sfx_price_peg.rate = 0;

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::create_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_rate_zero);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg rate is zero";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

    ASSERT_NO_THROW(this->m_db->close());

  }

  TYPED_TEST(SafexPricePegTest, UpdatePricePegExceptions) {
        boost::filesystem::path tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
        std::string dirPath = tempPath.string();

        this->set_prefix(dirPath);

        // make sure open does not throw
        ASSERT_NO_THROW(this->m_db->open(dirPath));
        this->get_filenames();
        this->init_hard_fork();

        for (int i = 0; i < NUMBER_OF_BLOCKS1; i++) {
            ASSERT_NO_THROW(this->m_db->add_block(this->m_blocks[i], this->m_test_sizes[i], this->m_test_diffs[i],
                                                  this->m_test_coins[i], this->m_test_tokens[i], this->m_txs[i]));
        }

        // Safex price peg doesn't exist
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::update_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", "username","USD","Some description", 1828);

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::update_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_not_existant);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg already exists";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

        // Safex price peg rate zero
        try
        {
          cryptonote::txin_to_script txinput = AUTO_VAL_INIT(txinput);
          txinput.command_type = safex::command_t::update_price_peg;
          txinput.key_offsets.push_back(0);

          safex::safex_price_peg sfx_price_peg = safex::safex_price_peg("Apple", this->m_safex_account1.username,"USD","Some description", 1828);

          sfx_price_peg.rate = 0;

          safex::create_price_peg_data price_peg_data{sfx_price_peg};

          safex::create_price_peg command1{SAFEX_COMMAND_PROTOCOL_VERSION , price_peg_data};


          safex::safex_command_serializer::serialize_safex_object(command1, txinput.script);

          std::unique_ptr<safex::command> command2 = safex::safex_command_serializer::parse_safex_object(txinput.script, safex::command_t::update_price_peg);

          safex::execution_status status = command2->validate(*(this->m_db), txinput);
          ASSERT_EQ(status, safex::execution_status::error_price_peg_rate_zero);

          std::unique_ptr<safex::execution_result> result{command2->execute(*(this->m_db), txinput)};
          FAIL() << "Should throw exception with Safex price peg rate zero";

        }
        catch (safex::command_exception &exception)
        {

        }
        catch (std::exception &exception)
        {
          FAIL() << "Exception happened " << exception.what();
        }
        catch (...)
        {
          FAIL() << "Unexpected exception";
        }

    ASSERT_NO_THROW(this->m_db->close());

  }

#endif

}  // anonymous namespace
