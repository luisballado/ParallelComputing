import os,sys
import subprocess

################################################################################
# Experiment configuration

# Test scenarios
problems = [
	"PROBLEM1",
	"PROBLEM2",
	"PROBLEM3",
]

# Where are problem files?
problem_path = "problems_directory"

# How many threads?
threads = [ 1,2,4,8 ]

# How many repetitions
runs = 5

# Where results will be stored?
results_path = "results"

################################################################################
# Run everything

for problem in problems:
	for cores in threads:

		# Create directory to store results
		path = results_path + "/%s/%d_cores" % (problem,cores)
		try:
			os.makedirs( path )
		except:
			pass

		for run in range(1,runs+1):
			
			torun = "./ejemplo %s/%s.txt %d" % (problem_path, problem, cores)
			output = "%s/%d.txt" % (path, run)
			print(torun)

			# Execute program
			with open(output, 'w') as fp:
				subprocess.run( torun.split(), stdout=fp )
			
################################################################################
