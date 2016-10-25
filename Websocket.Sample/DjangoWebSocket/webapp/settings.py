import os


HERE = os.path.dirname(os.path.abspath(__file__))
STATICFILES_DIRS = ( 'webapp/static', )
STATIC_URL = '/static/'
ROOT_URLCONF = 'webapp.urls'

LANGUAGE_CODE = 'zh-cn'
TIME_ZONE = 'UTC'
USE_TZ = USE_L10N = USE_I18N = True
TEMPLATE_DEBUG = DEBUG = True

INSTALLED_APPS = (
	'django.contrib.auth',
	'django.contrib.contenttypes',
	'django.contrib.sessions',
	'django.contrib.messages',
	'django.contrib.staticfiles',
)

MIDDLEWARE_CLASSES = (
	'django.contrib.sessions.middleware.SessionMiddleware',
	'django.middleware.common.CommonMiddleware',
	'django.middleware.csrf.CsrfViewMiddleware',
	'django.contrib.auth.middleware.AuthenticationMiddleware',
	'django.contrib.messages.middleware.MessageMiddleware',
	'django.middleware.clickjacking.XFrameOptionsMiddleware',
)

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'do5jwcy&6!b#e$v@ah63)ll-m2+w=kco!ru80wwsgampbovq++'
ALLOWED_HOSTS = []

