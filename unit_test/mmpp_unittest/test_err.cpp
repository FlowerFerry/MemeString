
#include <catch2/catch.hpp>

#include <megopp/err/err.hpp>

// ---------------------------------------------------------------------------
// 基础构造与 ok / operator bool
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err default constructor is ok", "[err]")
{
    mgpp::err e;
    REQUIRE(e.ok());
    REQUIRE(!e);
    REQUIRE(e.code() == MGEC__OK);
    REQUIRE(e.usercode() == 0);
    REQUIRE(e.message().empty());
}

TEST_CASE("mgpp::err code-only constructor", "[err]")
{
    mgpp::err e{ MGEC__ERR };
    REQUIRE(!e.ok());
    REQUIRE(!!e);
    REQUIRE(e.code() == MGEC__ERR);
    REQUIRE(e.message().empty());
}

TEST_CASE("mgpp::err code + message constructor", "[err]")
{
    mgpp::err e{ MGEC__ERR, "something went wrong" };
    REQUIRE(!e.ok());
    REQUIRE(e.code() == MGEC__ERR);
    REQUIRE(e.message() == "something went wrong");
    REQUIRE(e.solution().empty());
}

TEST_CASE("mgpp::err code + message + solution constructor", "[err]")
{
    mgpp::err e{ MGEC__ERR, "bad input", "check the value" };
    REQUIRE(e.message() == "bad input");
    REQUIRE(e.solution() == "check the value");
}

TEST_CASE("mgpp::err code + usercode constructor", "[err]")
{
    mgpp::err e{ MGEC__ERR, 42 };
    REQUIRE(e.code() == MGEC__ERR);
    REQUIRE(e.usercode() == 42);
}

// ---------------------------------------------------------------------------
// 复制与移动
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err copy constructor produces independent copy", "[err]")
{
    mgpp::err a{ MGEC__ERR, "original" };
    mgpp::err b{ a };
    REQUIRE(b.code() == MGEC__ERR);
    REQUIRE(b.message() == "original");
    b.set_message("modified");
    REQUIRE(a.message() == "original");
}

TEST_CASE("mgpp::err move constructor transfers ownership", "[err]")
{
    mgpp::err a{ MGEC__ERR, "move me" };
    mgpp::err b{ std::move(a) };
    REQUIRE(b.message() == "move me");
}

TEST_CASE("mgpp::err copy assignment produces independent copy", "[err]")
{
    mgpp::err a{ MGEC__ERR, "assign" };
    mgpp::err b;
    b = a;
    REQUIRE(b.message() == "assign");
    b.set_message("changed");
    REQUIRE(a.message() == "assign");
}

// ---------------------------------------------------------------------------
// set_message / set_solution / set_funcinfo
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err set_message on existing error", "[err]")
{
    mgpp::err e{ MGEC__ERR, "old" };
    e.set_message("new");
    REQUIRE(e.message() == "new");
}

TEST_CASE("mgpp::err set_solution upgrades to sln_err", "[err]")
{
    mgpp::err e{ MGEC__ERR, "msg" };
    REQUIRE(e.solution().empty());
    e.set_solution("fix it");
    REQUIRE(e.solution() == "fix it");
    REQUIRE(e.message() == "msg");
}

TEST_CASE("mgpp::err set_funcinfo", "[err]")
{
    mgpp::err e{ MGEC__ERR, "func error" };
    mgpp::err::fninfo fi;
    fi.name_ = "my_func";
    fi.args_[0] = "arg0_value";
    e.set_funcinfo(fi);
    REQUIRE(e.has_funcinfo());
    REQUIRE(e.get_funcinfo()->name_ == "my_func");
    REQUIRE(e.get_funcinfo()->args_.at(0) == "arg0_value");
}

// ---------------------------------------------------------------------------
// make_ok / make_unknown
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err make_ok is ok", "[err]")
{
    REQUIRE(mgpp::err::make_ok().ok());
}

TEST_CASE("mgpp::err make_unknown is not ok", "[err]")
{
    auto e = mgpp::err::make_unknown();
    REQUIRE(!e.ok());
    REQUIRE(e.code() == MGEC__ERR);
    REQUIRE(e.message() == "unknown");
}

// ---------------------------------------------------------------------------
// equality
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err operator== same code and category", "[err]")
{
    mgpp::err a{ MGEC__ERR, "msg a" };
    mgpp::err b{ MGEC__ERR, "msg b" };
    REQUIRE(a == b);
}

TEST_CASE("mgpp::err operator!= different codes", "[err]")
{
    mgpp::err a{ MGEC__ERR };
    mgpp::err b;
    REQUIRE(a != b);
}

// ---------------------------------------------------------------------------
// 错误链 — has_next / next / set_next 基础
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err has_next is false by default", "[err][chain]")
{
    mgpp::err e{ MGEC__ERR, "root" };
    REQUIRE(!e.has_next());
    REQUIRE(e.next().ok());
}

TEST_CASE("mgpp::err set_next attaches next error", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root error" };
    mgpp::err cause{ 1, "cause error" };
    root.set_next(cause);
    REQUIRE(root.has_next());
    mgpp::err n = root.next();
    REQUIRE(n.code() == 1);
    REQUIRE(n.message() == "cause error");
}

TEST_CASE("mgpp::err root code/message unchanged after set_next", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    mgpp::err cause{ 1, "cause" };
    root.set_next(cause);
    REQUIRE(root.code() == MGEC__ERR);
    REQUIRE(root.message() == "root");
}

TEST_CASE("mgpp::err set_next replaces existing next", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause1" });
    root.set_next(mgpp::err{ 2, "cause2" });
    REQUIRE(root.next().code() == 2);
    REQUIRE(root.next().message() == "cause2");
}

// ---------------------------------------------------------------------------
// 错误链 — 三层嵌套
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err three-level chain", "[err][chain]")
{
    mgpp::err e1{ 1, "level 1" };
    mgpp::err e2{ 2, "level 2" };
    mgpp::err e3{ 3, "level 3" };
    e2.set_next(e3);
    e1.set_next(e2);
    REQUIRE(e1.code() == 1);
    REQUIRE(e1.message() == "level 1");
    mgpp::err n1 = e1.next();
    REQUIRE(n1.code() == 2);
    REQUIRE(n1.message() == "level 2");
    mgpp::err n2 = n1.next();
    REQUIRE(n2.code() == 3);
    REQUIRE(n2.message() == "level 3");
    REQUIRE(!n2.has_next());
}

// ---------------------------------------------------------------------------
// 错误链 — 复制共享 chain 数据
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err copy of chained err shares next data", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause" });
    mgpp::err copy_of_root{ root };
    REQUIRE(copy_of_root.has_next());
    REQUIRE(copy_of_root.next().code() == 1);
    REQUIRE(copy_of_root.next().message() == "cause");
}

TEST_CASE("mgpp::err set_message COW after copy", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause" });
    mgpp::err copy_of_root{ root };
    copy_of_root.set_message("modified root");
    REQUIRE(root.message() == "root");
    REQUIRE(copy_of_root.message() == "modified root");
    REQUIRE(root.next().code() == 1);
    REQUIRE(copy_of_root.next().code() == 1);
}

TEST_CASE("mgpp::err set_solution COW keeps next accessible", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause" });
    mgpp::err copy_of_root = root;
    copy_of_root.set_solution("fix it");
    REQUIRE(copy_of_root.has_next());
    REQUIRE(copy_of_root.next().code() == 1);
    REQUIRE(root.solution().empty());
    REQUIRE(root.has_next());
}

// ---------------------------------------------------------------------------
// 错误链 — set_next COW
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err set_next COW: changing next on copy does not affect original", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause1" });
    mgpp::err copy_of_root = root;
    copy_of_root.set_next(mgpp::err{ 2, "cause2" });
    REQUIRE(root.next().code() == 1);
    REQUIRE(copy_of_root.next().code() == 2);
}

// ---------------------------------------------------------------------------
// 错误链 — chain 上的 set_funcinfo
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err set_funcinfo on chained err", "[err][chain]")
{
    mgpp::err root{ MGEC__ERR, "root" };
    root.set_next(mgpp::err{ 1, "cause" });
    mgpp::err::fninfo fi;
    fi.name_ = "some_fn";
    root.set_funcinfo(fi);
    REQUIRE(root.has_funcinfo());
    REQUIRE(root.get_funcinfo()->name_ == "some_fn");
    REQUIRE(root.has_next());
    REQUIRE(root.next().code() == 1);
}

// ---------------------------------------------------------------------------
// 错误链 — ok 的 err 调用 set_next
// ---------------------------------------------------------------------------

TEST_CASE("mgpp::err ok err set_next attaches chain", "[err][chain]")
{
    mgpp::err root;
    root.set_next(mgpp::err{ 1, "cause" });
    REQUIRE(root.has_next());
    REQUIRE(root.next().code() == 1);
    REQUIRE(root.ok());
}
