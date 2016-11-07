-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Feb 11, 2013 at 11:00 AM
-- Server version: 5.5.29
-- PHP Version: 5.4.6-1ubuntu1.1

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `harusdb`
--
CREATE DATABASE `harusdb` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `harusdb`;

-- --------------------------------------------------------

--
-- Table structure for table `ClusterData`
--

CREATE TABLE IF NOT EXISTS `ClusterData` (
  `id_cluster` int(11) NOT NULL,
  `data` text NOT NULL,
  `compression_rate` int(11) NOT NULL,
  `num_clusters` int(11) NOT NULL,
  PRIMARY KEY (`id_cluster`,`compression_rate`,`num_clusters`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `DetectionResults`
--

CREATE TABLE IF NOT EXISTS `DetectionResults` (
  `inference_type` varchar(100) NOT NULL COMMENT 'HMM OlmoAutomata etc',
  `data_type` int(11) NOT NULL COMMENT 'Training, Test, Validation',
  `compression_rate` int(11) NOT NULL,
  `num_clusters` int(11) NOT NULL,
  `errors_by_action` text NOT NULL,
  `errors_by_action_selected` text NOT NULL,
  PRIMARY KEY (`inference_type`,`data_type`,`compression_rate`,`num_clusters`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `HMMDataSet`
--

CREATE TABLE IF NOT EXISTS `HMMDataSet` (
  `num_clusters` int(11) NOT NULL,
  `compression_rate` int(11) NOT NULL,
  `id_action` int(11) NOT NULL,
  `state_trans` text NOT NULL COMMENT 'num_states x num_states',
  `initial_state_dist` text NOT NULL COMMENT 'num_states',
  `obs_prob_dist` text NOT NULL COMMENT 'num_states x num_states',
  `cluster_distribution` text NOT NULL COMMENT 'num_states',
  PRIMARY KEY (`num_clusters`,`compression_rate`,`id_action`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `HarusDataSet`
--

CREATE TABLE IF NOT EXISTS `HarusDataSet` (
  `id_action` int(11) NOT NULL,
  `data` text NOT NULL,
  `type` int(11) NOT NULL COMMENT 'Training, Test, Validation',
  `id` int(11) NOT NULL,
  `compression_rate` int(11) NOT NULL,
  PRIMARY KEY (`type`,`id`,`compression_rate`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `OlmoAutomataDS`
--

CREATE TABLE IF NOT EXISTS `OlmoAutomataDS` (
  `num_clusters` int(11) NOT NULL,
  `compression_rate` int(11) NOT NULL,
  `id_action` int(11) NOT NULL,
  `transition_matrix` text NOT NULL COMMENT 'num_states x num_states',
  `init_states` text NOT NULL COMMENT 'num_states',
  `final_states` text NOT NULL COMMENT 'num_states',
  `sequence_length` int(11) NOT NULL,
  `min_punt_belong` double NOT NULL,
  PRIMARY KEY (`num_clusters`,`compression_rate`,`id_action`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `SimpleDataSet`
--

CREATE TABLE IF NOT EXISTS `SimpleDataSet` (
  `id` int(11) NOT NULL,
  `id_action` int(11) NOT NULL,
  `compression_rate` int(11) NOT NULL,
  `num_clusters` int(11) NOT NULL,
  `sequence` text NOT NULL COMMENT 'discret sequence',
  `type` int(11) NOT NULL DEFAULT '1' COMMENT 'Training, Test, Validation',
  PRIMARY KEY (`id`,`id_action`,`compression_rate`,`num_clusters`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
