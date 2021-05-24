/**
 * Shadow Daemon -- Web Application Firewall
 *
 *   Copyright (C) 2014-2021 Hendrik Buchwald <hb@zecure.org>
 *
 * This file is part of Shadow Daemon. Shadow Daemon is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "integrity.h"

BOOST_AUTO_TEST_SUITE(integrity_test)

BOOST_AUTO_TEST_CASE(positive_integrity_check) {
    swd::cache_ptr cache(new swd::cache(swd::database_ptr()));
    swd::integrity integrity(cache);

    swd::request_ptr request(new swd::request);
    swd::profile_ptr profile(new swd::profile);
    profile->set_id(1);
    request->set_profile(profile);
    request->set_caller("qux");
    request->add_hash("boo", "far");

    swd::integrity_rule_ptr rule(new swd::integrity_rule);
    rule->set_algorithm("foo");
    rule->set_digest("bar");

    swd::integrity_rules rules;
    rules.push_back(rule);
    cache->add_integrity_rules(1, "qux", rules);

    integrity.scan(request);
    BOOST_CHECK(request->get_integrity_rules().size() == 1);
    BOOST_CHECK(request->is_threat() == true);
}

BOOST_AUTO_TEST_CASE(negative_integrity_check) {
    swd::cache_ptr cache(new swd::cache(swd::database_ptr()));
    swd::integrity integrity(cache);

    swd::request_ptr request(new swd::request);
    swd::profile_ptr profile(new swd::profile);
    profile->set_id(1);
    request->set_profile(profile);
    request->set_caller("qux");
    request->add_hash("foo", "bar");

    swd::integrity_rule_ptr rule(new swd::integrity_rule);
    rule->set_algorithm("foo");
    rule->set_digest("bar");

    swd::integrity_rules rules;
    rules.push_back(rule);
    cache->add_integrity_rules(1, "qux", rules);

    integrity.scan(request);
    BOOST_CHECK(request->get_integrity_rules().size() == 0);
    BOOST_CHECK(request->is_threat() == false);
}

BOOST_AUTO_TEST_SUITE_END()
