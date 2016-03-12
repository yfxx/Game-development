package com.game.test;

import com.game.rabbitmq.*;

public class Clienttest1 {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MqConfig mycfg = MqConfig.getInstance();
		mycfg.setExchangeName("ServerExchange");
		mycfg.setIp("192.168.40.1");
		mycfg.setPort(5672);
		mycfg.setUser("admin");
		mycfg.setPassword("admin");
		mycfg.setQueueName("nodeId_1");
		MqReceiver mqReci = new MqReceiver();
		//mqReci.setMqConfig(mycfg);
		mqReci.loopReceiveMessage();
		
	}
}
