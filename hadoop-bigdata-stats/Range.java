import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Range {

	public static class CategoryRangeMapper extends Mapper<Object, Text, Text, Text> {
		public static int keyType = 4;
		private Text val = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String valString = tokens[i];
					if (valString.equalsIgnoreCase(""))
						valString = "0";
					else if (valString.equalsIgnoreCase("price"))
						continue;
					val.set(valString + "," + valString);
					context.write(word, val);
				}
			}
		}	
	}
	
	public static class BrandRangeMapper extends Mapper<Object, Text, Text, Text> {
		public static int keyType = 5;
		private Text val = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String valString = tokens[i];
					if (valString.equalsIgnoreCase(""))
						valString = "0";
					else if (valString.equalsIgnoreCase("price"))
						continue;
					val.set(valString + "," + valString);
					context.write(word, val);
				}
			}
		}	
	}
	
	public static class EventRangeMapper extends Mapper<Object, Text, Text, Text> {
		public static int keyType = 1;
		private Text val = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String valString = tokens[i];
					if (valString.equalsIgnoreCase(""))
						valString = "0";
					else if (valString.equalsIgnoreCase("price"))
						continue;
					val.set(valString + "," + valString);
					context.write(word, val);
				}
			}
		}	
	}
	
	public static class RangeCombiner extends Reducer<Text, Text, Text, Text> {
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			float min = Float.MAX_VALUE;
			float max = 0;
			float current = 0;
			for (Text t : values) {
				StringTokenizer itr = new StringTokenizer(t.toString(), ",");
				current = Float.parseFloat(itr.nextToken());
				if (current < min) {
					min = current;
				}
				current = Float.parseFloat(itr.nextToken());
				if (current > max) {
					max = current;
				}
			}
			context.write(key, new Text(min + "," + max));
		}
	}
	
	public static class RangeReducer extends Reducer<Text, Text, Text, Text> {
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			float min = Float.MAX_VALUE;
			float max = 0;
			float current = 0;
			for (Text t : values) {
				StringTokenizer itr = new StringTokenizer(t.toString(), ",");
				current = Float.parseFloat(itr.nextToken());
				if (current < min) {
					min = current;
				}
				current = Float.parseFloat(itr.nextToken());
				if (current > max) {
					max = current;
				}
			}
			context.write(key, new Text(max - min + ""));
		}
	}
	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		System.out.println("Range Mode");
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "find range");
		job.setJarByClass(Range.class);
		if (Integer.parseInt(args[0]) == 1) {
			//Category
			job.setMapperClass(CategoryRangeMapper.class);
		} else if (Integer.parseInt(args[0]) == 2) {
			//Brand
			job.setMapperClass(BrandRangeMapper.class);
		} else {
			//Event
			job.setMapperClass(EventRangeMapper.class);
		}
		job.setCombinerClass(RangeCombiner.class);
		job.setReducerClass(RangeReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		Path input = new Path(args[1]);
		Path output = new Path(args[2]);
		FileSystem hdfs = FileSystem.get(conf);
		if (hdfs.exists(output))
		      hdfs.delete(output, true);
		FileInputFormat.addInputPath(job, input);
		FileOutputFormat.setOutputPath(job, new Path(args[2]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
	
}

