var myChart1 = echarts.init(document.getElementById('main'));
 // 指定图表的配置项和数据

 let echat_data = [];
 let echat_value_t = [];
 let echat_value_h = [];



  // 使用刚指定的配置项和数据显示图表。
 

  var myChart = echarts.init(document.getElementById('lastdata'));


  const width = window.innerWidth;

  if(width > 600)
  {
    myChart1.resize({
      width: width,
      height: 600
    });
    myChart.resize({
      width: width,
      height: 600
    });
  }
  else
  {
    myChart1.resize({
      width: width,
      height: 720
    });
    myChart.resize({
      width: width,
      height: 720
    });
  }

  var option1 = {
    title: {
      text: '湿度变化曲线',
      left: 'center'
    },
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross'
      }
    },
    toolbox: {
      show: true,
      feature: {
        saveAsImage: {}
      }
    },
  
    xAxis: {
      data: echat_data,
      // type: 'category',
    },
    yAxis: {    
      type: 'value',
      name: '温度',
      axisLabel: {
        formatter: '{value} %'
      },
      axisPointer: {
        snap: true
      }
    },
    series: [
      {
        name:'温度',
        data: echat_value_h,
        type: 'line',
        showSymbol: false,
        yAxisIndex: 0,
        lineStyle: {
          normal: {
            color: '#735E0D',
            width: 2,
          }
        },
      }
    ]
  };


 // 指定温度配置
 var option2 = {
  title: {
    text: '温度变化曲线',
    left: 'center'
  },
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'cross'
    }
  },
  toolbox: {
    show: true,
    feature: {
      saveAsImage: {}
    }
  },

  xAxis: {
    data: echat_data,
    // type: 'category',
  },
  yAxis: {    
    type: 'value',
    name: '温度',
    axisLabel: {
      formatter: '{value} °'
    },
    axisPointer: {
      snap: true
    }
  },
  series: [
    {
      name:'温度',
      data: echat_value_t,
      type: 'line',
      showSymbol: false,
      yAxisIndex: 0,
      lineStyle: {
        normal: {
          color: '#4C256E',
          width: 2,
        }
      },
    }
  ]
};





async function fetchdata() {
  try{
      const response = await fetch("http://192.168.31.26:1024/getalldata");
      if(!response.ok) {
          throw new Error("网络不响应");
      }
      const data = await response.json();

      const bak =   data.length;
      let i;
      if(bak > 1000)
      {
        i = 1000;
      }
      else 
      {
        i = 0;
      }
      for(i;i < bak;i++){
        const key1 = "温度";
        const key4 = "湿度";
        const key2 = "时间";
        const key3 = "日期";
        
        const temp = data[i][key1];
        const date = data[i][key3] + ' ' + data[i][key2];
        const h = data[i][key4];

        echat_value_t.push(temp);
        echat_value_h.push(h);
        echat_data.push(date);
        // if(i > 100)
        //   break;

      }


      console.log(echat_value_t);
       option2.xAxis.data = echat_data;
       option2.series[0].data = echat_value_t;
       option1.series[0].data = echat_value_h;
       option1.xAxis.data = echat_data;

       myChart.hideLoading();
       myChart1.hideLoading();
       myChart.setOption(option2);
       myChart1.setOption(option1);

  } catch(error) {
      console.error("请求失败：",error);
  }
}


myChart.showLoading();
myChart1.showLoading();
fetchdata();



