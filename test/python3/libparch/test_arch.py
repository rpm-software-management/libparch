import unittest

import libparch.arch


class TestArch(unittest.TestCase):

    archinfo = libparch.arch.libparch_init()

    def test_x86_64_name(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        self.assertEqual(libparch.arch.libparch_arch_get_name(arch), "x86_64")

    def test_x86_64_bits(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        self.assertEqual(libparch.arch.libparch_arch_get_bits(arch), 64)

    def test_x86_64_is_multilib(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        self.assertEqual(libparch.arch.libparch_arch_is_multilib(arch), True)

    def test_x86_64_is_source(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        self.assertEqual(libparch.arch.libparch_arch_is_source(arch), False)

    def test_x86_64_is_noarch(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        self.assertEqual(libparch.arch.libparch_arch_is_noarch(arch), False)

    def test_x86_64_compatible_native_arches(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        actual = libparch.arch.libparch_arch_get_compatible_native_arches(arch)
        expected = ("x86_64", "noarch")
        self.assertEqual(expected, actual)

    def test_x86_64_compatible_multilib_arches(self):
        arch = libparch.arch.libparch_get_arch(self.archinfo, "x86_64")
        actual = libparch.arch.libparch_arch_get_compatible_multilib_arches(arch)
        expected = ("athlon", "i686", "i586", "i486", "i386")
        self.assertEqual(expected, actual)

    def test_autodetect(self):
        arch_name = libparch.arch.libparch_detected_arch_name(self.archinfo)
        arch = libparch.arch.libparch_detected_arch(self.archinfo)
        os = libparch.arch.libparch_detected_os(self.archinfo)
        self.assertEqual(arch_name, "x86_64")
        self.assertTrue(arch)

    def test_canon(self):
        arch = libparch.arch.libparch_arch_get_canonical_name("x86_64")
        self.assertEqual(arch, "x86_64")
        arch = libparch.arch.libparch_arch_get_canonical_name("riscv")
        self.assertEqual(arch, "riscv64")
        os = libparch.arch.libparch_os_get_canonical_name("BSD_OS")
        self.assertEqual(os, "bsdi")
        os = libparch.arch.libparch_os_get_canonical_name("NONEXISTANT")
        self.assertEqual(os, "NONEXISTANT")
