import unittest
import subprocess

class BBT_Simple(unittest.TestCase):

    def setUp(self):
        self.dn = file("/tmp/null", "w")

    def run_process(self, sinput):
        binary = "bin/statsd"
        p = subprocess.Popen(
            [ binary ],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE, stderr=self.dn)
        p.stdin.write(sinput)
        p.stdin.close()
        l = p.stdout.readlines()
        p.wait()
        return p, l

    def bbt_run1(self):
        p, l = self.run_process("a:1|c")

        self.assertEqual(['a: 1\n'], l)
