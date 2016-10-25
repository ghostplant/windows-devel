using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Collections.Generic;
using System.Reflection.Emit;
using System.Reflection;

namespace 语法分析器
{

    #region 数据源转换静态类 DataSourceCreator.ToDataSource
    public static class DataSourceCreator
    {
        public static List<object> ToDataSource(List<KeyValuePair<string, List<string>>> list)
        {
            // 创建属性列标题
            TypeBuilder tb = GetTypeBuilder(list.GetHashCode());
            foreach (var pair in list)
                CreateProperty(tb, pair.Key, pair.Key.GetType());
            Type objectType = tb.CreateType();

            // 创建表格行记录
            var itemsSource = new List<object>();
            int nRow = 0;
            foreach (var pair in list)
                nRow = Math.Max(nRow, pair.Value.Count);
            for (int i = 0; i < nRow; i++)
                itemsSource.Add(Activator.CreateInstance(objectType));

            // 设置表格单元数据
            foreach (var pair in list)
            {
                PropertyInfo property = objectType.GetProperty(pair.Key.ToString());
                for (int i = 0; i < pair.Value.Count; i++)
                    property.SetValue(itemsSource[i], Convert.ChangeType(pair.Value[i], property.PropertyType, null), null);
            }
            return itemsSource;
        }

        private static TypeBuilder GetTypeBuilder(int code)
        {
            AssemblyName an = new AssemblyName("TempAssembly" + code);
            AssemblyBuilder assemblyBuilder = AppDomain.CurrentDomain.DefineDynamicAssembly(an, AssemblyBuilderAccess.Run);
            ModuleBuilder moduleBuilder = assemblyBuilder.DefineDynamicModule("MainModule");
            TypeBuilder tb = moduleBuilder.DefineType("TempType" + code, TypeAttributes.Public, typeof(object));
            return tb;
        }

        private static void CreateProperty(TypeBuilder tb, string propertyName, Type propertyType)
        {
            FieldBuilder fieldBuilder = tb.DefineField("_" + propertyName, propertyType, FieldAttributes.Private);

            PropertyBuilder propertyBuilder = tb.DefineProperty(propertyName, PropertyAttributes.HasDefault, propertyType, null);
            MethodBuilder getPropMthdBldr = tb.DefineMethod("get_" + propertyName, MethodAttributes.Public, propertyType, Type.EmptyTypes);
            ILGenerator getIL = getPropMthdBldr.GetILGenerator();
            getIL.Emit(OpCodes.Ldarg_0);
            getIL.Emit(OpCodes.Ldfld, fieldBuilder);
            getIL.Emit(OpCodes.Ret);
            MethodBuilder setPropMthdBldr = tb.DefineMethod("set_" + propertyName, MethodAttributes.Public, null, new Type[] { propertyType });
            ILGenerator setIL = setPropMthdBldr.GetILGenerator();
            setIL.Emit(OpCodes.Ldarg_0);
            setIL.Emit(OpCodes.Ldarg_1);
            setIL.Emit(OpCodes.Stfld, fieldBuilder);
            setIL.Emit(OpCodes.Ret);
            propertyBuilder.SetGetMethod(getPropMthdBldr);
            propertyBuilder.SetSetMethod(setPropMthdBldr);
        }
    }
    #endregion
}
