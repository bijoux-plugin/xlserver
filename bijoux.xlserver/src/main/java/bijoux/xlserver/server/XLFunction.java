package bijoux.xlserver.server;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface XLFunction {
    String name() default "";

    String help() default "";

    String[] args() default {};

    String[] argHelp() default {};

    String category() default "";

    boolean isVolatile() default false;
}
