#include "sqlite_keychain.hpp"
#include <assert.h>

void TestKeychain()
{
	SqliteKeychain keychain;

	assert(keychain.getNextID() == 1);

	beam::Coin coin1(keychain.getNextID(), 5);
	keychain.store(coin1);

	beam::Coin coin2(keychain.getNextID(), 2);
	keychain.store(coin2);

	assert(keychain.getNextID() == 3);

	auto coins = keychain.getCoins(7);

	assert(coins.size() == 2);

	{
		std::vector<beam::Coin> localCoins;
		localCoins.push_back(coin2);
		localCoins.push_back(coin1);

		for (int i = 0; i < coins.size(); ++i)
		{
			assert(localCoins[i].m_id == coins[i].m_id);
			assert(localCoins[i].m_amount == coins[i].m_amount);
			assert(localCoins[i].m_status == coins[i].m_status);
		}
	}

	{
		std::vector<beam::Coin> coins;
		coin2.m_status = beam::Coin::Spent;
		coins.push_back(coin2);

		keychain.update(coins);

		assert(keychain.getCoins(7).size() == 1);
	}
}

int main() {

	TestKeychain();
}
