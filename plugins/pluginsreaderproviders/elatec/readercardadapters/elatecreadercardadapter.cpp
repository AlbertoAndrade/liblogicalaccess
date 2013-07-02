/**
 * \file elatecreadercardadapter.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Elatec reader/card adapter.
 */

#include "elatecreadercardadapter.hpp"

namespace logicalaccess
{		
	ElatecReaderCardAdapter::ElatecReaderCardAdapter()
		: ReaderCardAdapter()
	{
		
	}

	ElatecReaderCardAdapter::~ElatecReaderCardAdapter()
	{
		
	}

	unsigned char ElatecReaderCardAdapter::calcChecksum(const std::vector<unsigned char>& buf) const
	{
		unsigned char checksum = 0x00;

		for (size_t i = 0; i < buf.size(); ++i)
		{
			checksum = checksum ^ buf[i];
		}

		return checksum;
	}

	std::vector<unsigned char> ElatecReaderCardAdapter::adaptCommand(const std::vector<unsigned char>& command)
	{
		EXCEPTION_ASSERT_WITH_LOG(command.size() >= 1, std::invalid_argument, "A valid command buffer size must be at least 1 byte long");

		unsigned char tmpcmd[3];
		tmpcmd[0] = command[0];
		tmpcmd[1] = 0xFF;	// all readers
		tmpcmd[2] = static_cast<unsigned char>(command.size() + 2);
		std::vector<unsigned char> cmd(tmpcmd, tmpcmd + 3);
		cmd.insert(cmd.end(), command.begin() + 1, command.end());
		cmd.push_back(calcChecksum(cmd));
		d_last_cmdcode = tmpcmd[0];	

		return cmd;
	}

	std::vector<unsigned char> ElatecReaderCardAdapter::sendCommand(unsigned char cmdcode, const std::vector<unsigned char>& command, long int timeout)
	{
		std::vector<unsigned char> cmd = command;
		cmd.insert(cmd.begin(), cmdcode);
		return ReaderCardAdapter::sendCommand(cmd, timeout);
	}

	std::vector<unsigned char> ElatecReaderCardAdapter::adaptAnswer(const std::vector<unsigned char>& answer)
	{
		std::vector<unsigned char> ret;
		EXCEPTION_ASSERT_WITH_LOG(answer.size() >= 5, std::invalid_argument, "A valid buffer size must be at least 5 bytes long");
		
		unsigned char buflength = answer[0];
		EXCEPTION_ASSERT_WITH_LOG(answer.size() >= buflength, std::invalid_argument, "The buffer is too short according to the excepted buffer length");

		EXCEPTION_ASSERT_WITH_LOG(answer[2] == d_last_cmdcode, std::invalid_argument, "The request type doesn't match the command sent by the host");

		unsigned char status = answer[3];
		// Don't throw exception for "no card" when getting card list
		if (d_last_cmdcode != 0x11 && (status != 0x01))
		{
			EXCEPTION_ASSERT_WITH_LOG(status == 0x00, std::invalid_argument, "Elatec error");
		}

		unsigned char checksum = answer[buflength-1];
		EXCEPTION_ASSERT_WITH_LOG(calcChecksum(std::vector<unsigned char>(answer.begin(), answer.begin() + buflength - 1)) == checksum, std::invalid_argument, "Wrong checksum");

		ret = std::vector<unsigned char>(answer.begin() + 4, answer.begin() + 4 + buflength - 5);
		
		return ret;
	}
}
