package com.game.rabbitmq;

import java.io.IOException;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang3.SerializationUtils;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.MessageProperties;

public class MqSender extends MqManager{
	
	public void exit() {
		closeConnection();
	}
	
	public int send2All(Command command)
	{
		try {
			Map<String, String> message = new HashMap<String,String>();
			message.put("type",command.getType().toString());
			message.put("date", command.getDate().toString());
			sendMessage("all",(Serializable) message);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return 0;
	}
	public int sendCommand(String nodeId,Command command)
	{
		try {
			Map<String, String> message = new HashMap<String,String>();
			message.put("type",command.getType().toString());
			message.put("commandId", command.getCommandId());
			message.put("taskId",command.getTaskId());
			message.put("taskName", command.getTaskName());
			//message.put("date", command.getDate().toString());
			sendMessage("nodeId_"+nodeId,(Serializable) message);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return 0;
	}
	
	protected void sendMessage(String routingKey,Serializable object) throws IOException {
		Channel channel = getChannel();
		if (channel == null)
			return;

		try {
			channel.exchangeDeclare(this.mqConfig.getExchangeName(), "direct" );
			channel.basicPublish(this.mqConfig.getExchangeName(),routingKey, MessageProperties.PERSISTENT_TEXT_PLAIN, SerializationUtils.serialize(object));
			System.out.println(" [x] Sent '" + object.toString() + "'");
			channel.close();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
    } 
}
