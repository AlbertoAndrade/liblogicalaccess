/**
 * \file desfireiso7816resultchecker.hpp
 * \author Arnaud H. <arnaud-dev@islog.com>
 * \brief DESFire ISO 7816 Result checker.
 */

#ifndef LOGICALACCESS_DESFIREISO7816RESULTCHECKER_HPP
#define LOGICALACCESS_DESFIREISO7816RESULTCHECKER_HPP

#include <logicalaccess/plugins/readers/iso7816/iso7816resultchecker.hpp>

namespace logicalaccess
{
/**
 * \brief The DESFire ISO 7816 result checker.
 */
class LLA_READERS_ISO7816_API DESFireISO7816ResultChecker : public ISO7816ResultChecker
{
  public:
    DESFireISO7816ResultChecker();

    virtual ~DESFireISO7816ResultChecker();
};
}

#endif