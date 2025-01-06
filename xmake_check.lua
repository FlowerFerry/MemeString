
-- function check_c_compiler_flag(flag)
--     import("core.tool.compiler")

--     local test_file = path.join(os.tmpdir(), "check_c_compiler_flag_test.c")
--     local test_prog = path.join(os.tmpdir(), "check_c_compiler_flag_test")

--     io.writefile(test_file, "int main(int argc, char **argv) { return 0; }")

--     local ok = compiler.compile(test_file, test_prog, { flags = {flag} })

--     os.rm(test_file)
--     os.rm(test_prog)

--     return ok
-- end
