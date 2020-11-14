# Multistage Coil Gun

## Introduction

Arduino ESP32 code and schematic for a 300V 3 stage electromagnetic coil gun. A video showing a test fire is located at https://youtu.be/OdKDb0fnAb0

This project shows the basic use of electromagnetic force to propel a projectile ball faster and faster through multiple coils that are fired as soon as the projectile reaches the next coil.  This design has not optimized the timing or "suck back" effect, and is merely to show the fundamentals of how a coil gun operates.

## Safety

CAUTION! This project uses large capacitors and a high voltage power supply.  Use extreme caution to ensure that the high voltage connections are insulated from touching, and always ensure the capacitors are properly discharged before modifying or touching. Do not attempt this project if you are not experienced with high voltage DC circuits.

## Parts List
Base
-	Blue cutting board					$20.00 each
-	10mm clear acrylic sheet				$10.00 each
-	Clear acrylic pipe – 20mm OD 16mm ID x 1 meter long	$18.76 each
Charging circuit
-	1x Devmo HV boost converter 8-32V IN to 45-390V OUT	$17.05 each
-	6x 850uF 400V Capacitors				$6.47 each
-	1x 4.7kOhm resister 5Watt				$0.48 each
-	1x 4PDT toggle switch  ST0506				$8.95 each
-	3x 460 Ohm resisters
Firing circuit
-	3x spools enamel copper wire 0.8mm x 22m		$10.00 each
-	3x 600V 55A SCR Thyristor S6055MTP			$12.51 each
-	3x Diode (1N5404) 400V 3A				$0.17 each
-	3x 9volt Batteries					$1.00 each
-	3x optocouplers 4N25					$1.75 each
-	1x Arduino ESP32 microcontroller			$20.00 each
-	6x 120 Ohm resisters
-	3x Red LED
Speedometer & Trigger sensors
-	4x Infrared transmitting LED ZD1945			$1.75 each
-	4x Infrared receiver (phototransistor) ZD1950		$1.75 each
-	4x 110 Ohm resistors
-	4x 22 kOhm resistors
Voltmeter
-	4x LED voltmeters BY436V				$2.50 each
-	1x 9volt Battery						$1.00 each
Projectile
-	15mm ferrous steel ball (stainless steel won’t work)
