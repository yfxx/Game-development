package com.game.rabbitmq;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

public abstract class MqManager {
	protected MqConfig mqConfig;
	private Connection connection;
	private Channel channel;
	public MqManager()
	{
		this.mqConfig = MqConfig.getInstance();
	}
	
	public void setMqConfig(MqConfig mqConfig)
	{
		this.mqConfig = mqConfig;
	}

	protected Connection getConnection()
	{
		if ((this.connection == null) || (!(this.connection.isOpen())))
			this.connection = connect();

		return this.connection;
	}

	protected void closeConnection() 
	{
		if (this.connection != null)
			try {
				this.connection.close();
			} catch (IOException e) {
				throw new RuntimeException("close mq connection error", e);
			}
	}

	protected Channel getChannel()
	{
		if ((this.channel != null) && (this.channel.isOpen())) {
			return this.channel;
	    }

	    Connection connection = getConnection();
	    if (connection == null)
	    	return null;

	    try
	    {
	    	this.channel = connection.createChannel();
	    	channel.exchangeDeclare(this.mqConfig.getExchangeName(), "direct" );
	    	//boolean durable = true;
	    	//this.channel.queueDeclare(this.mqConfig.getQueueName(), durable, false, false, null);
	    	return this.channel;
	    } catch (IOException e) {
	    	throw new RuntimeException("mq error", e);
	    }
	}
	
	protected Connection connect() 
	{
		try {
			ConnectionFactory factory = new ConnectionFactory();
			factory.setHost(this.mqConfig.getIp());
			factory.setPort(this.mqConfig.getPort());
			factory.setUsername(this.mqConfig.getUser());
			factory.setPassword(this.mqConfig.getPassword());
			factory.setConnectionTimeout(10000);

			System.out.println("connect to mq:[" + this.mqConfig.getIp() + ":" + this.mqConfig.getPort() + ","
					+ this.mqConfig.getUser() + "/" + this.mqConfig.getPassword());
			Connection conn = factory.newConnection();
			System.out.println("success connect to mq:[" + this.mqConfig.getIp() + ":" + this.mqConfig.getPort() + ","
					+ this.mqConfig.getUser() + "/" + this.mqConfig.getPassword());
			return conn;
		} catch (IOException | TimeoutException e) {
			do {
				try {
					Thread.sleep(30000L);
				} catch (InterruptedException e1) {
					Thread.currentThread().interrupt();
				}
			} while (!(Thread.currentThread().isInterrupted()));

			Thread.interrupted();
		}
		return null;
	}
}
