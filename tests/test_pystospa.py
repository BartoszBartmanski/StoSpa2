#!/usr/bin/env python

import pystospaboost as pystospa
import unittest


class TestReaction(unittest.TestCase):

    def test_constructor(self):
        # Create a Reaction object
        r = pystospa.Reaction(0.0, lambda x,y : 1.0, [0])

        # Check that Reaction object attributes have been set correctly
        self.assertEqual(r.get_rate(), 0)
        self.assertEqual(r.get_propensity([10], 1.0), 0.0)

    def test_member_functions(self):
        # Create a Reaction object
        r = pystospa.Reaction(0.0, lambda x,y : 1.0, [0])

        # Check that reaction rate can be changed
        r.set_rate(1.55)
        self.assertEqual(r.get_rate(), 1.55)

        # Check that the correct propensity is returned
        self.assertEqual(r.get_propensity([10], 1.0), 1.55)


class TestVoxel(unittest.TestCase):

    def test_constructor(self):

        # Create a Voxel object
        v = pystospa.Voxel([10], 1.0)

        # Check that the Voxel attributes are as expected
        self.assertEqual(len(v.get_molecules()), 1)
        self.assertEqual(v.get_molecules()[0], 10)
        self.assertEqual(v.get_voxel_size(), 1.0)

    def test_member_functions(self):

        # Create a Voxel object
        v = pystospa.Voxel([10], 1.0)
        r = pystospa.Reaction(1.5, lambda x,y : x[0], [-1])

        # Check that a Reaction object has been added correctly
        v.add_reaction(r)
        rs = v.get_reactions()
        self.assertEqual(len(rs), 1)
        self.assertEqual(rs[0].get_rate(), 1.5)
        self.assertEqual(v.get_total_propensity(), 15)


class TestSimulator(unittest.TestCase):

    def test_constructor(self):

        # Create a Simulator object
        v = pystospa.Voxel([10], 1.0)
        v.add_reaction(pystospa.Reaction(1.5, lambda x,y : x[0], [-1]))
        s = pystospa.Simulator([v])

        # Check that the Simulator attributes are as expected
        self.assertEqual(s.get_time(), 0.0)
        self.assertEqual(s.get_voxels()[0].get_molecules()[0], 10)

    def test_member_functions(self):

        # Create a Simulator object
        v = pystospa.Voxel([10], 1.0)
        v.add_reaction(pystospa.Reaction(1.5, lambda x,y : x[0], [-1]))
        s = pystospa.Simulator([v])

        # Check setting seed for generating random number
        s.set_seed(153)
        self.assertEqual(s.get_seed(), 153)

        # Check a single step in SSA works
        s.step()
        self.assertGreater(s.get_time(), 0)
        self.assertEqual(s.get_voxels()[0].get_molecules()[0], 9)

        # Check that getting to specific time point works
        s.advance(1.0)
        self.assertGreater(s.get_time(), 1.0)


if __name__ == '__main__':
    unittest.main()
